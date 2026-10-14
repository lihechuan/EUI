[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtDir,
    [string]$BuildDir,
    [string]$OutputDir,
    [string]$CrtDir,
    [switch]$Refresh,
    [string]$GalleryBuildFile,
    [string]$CMakeProgram = 'cmake.exe'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Invoke-Checked {
    param([string]$Program, [string[]]$Arguments)
    & $Program @Arguments
    if ($LASTEXITCODE -ne 0) { throw "$Program failed: $LASTEXITCODE" }
}

$repository = Split-Path $PSScriptRoot -Parent
$QtDir = (Resolve-Path -LiteralPath $QtDir).Path
if (-not $BuildDir) { $BuildDir = Join-Path $repository 'build/quick3d-gallery-runtime' }
if (-not $OutputDir) { $OutputDir = Join-Path $repository 'build/quick3d-runtime' }
$OutputDir = [IO.Path]::GetFullPath($OutputDir)
$manifestPath = Join-Path $OutputDir 'manifest.json'
$expectedBuild = if ($GalleryBuildFile) { Get-Content -LiteralPath $GalleryBuildFile -Raw | ConvertFrom-Json } else { $null }
if (Test-Path -LiteralPath $OutputDir) {
    if ((Get-Item -LiteralPath $OutputDir).Attributes -band [IO.FileAttributes]::ReparsePoint) {
        throw 'The runtime output must not be a symbolic link or junction.'
    }
    if (Get-ChildItem -LiteralPath $OutputDir -Force | Select-Object -First 1) {
        if (-not $Refresh -or -not (Test-Path -LiteralPath $manifestPath -PathType Leaf)) {
            throw 'Use an empty output directory or -Refresh with an existing runtime payload.'
        }
        $previousManifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
        if ($previousManifest.schema -ne 1 -or $previousManifest.architecture -ne 'x64' -or
            $previousManifest.entry -ne 'EUI-Gallery-Qt6.exe') {
            throw 'Only an existing EUI Qt 6 runtime payload can be refreshed.'
        }
        if ($expectedBuild -and $previousManifest.PSObject.Properties.Name -contains 'galleryVersion' -and
            $previousManifest.PSObject.Properties.Name -contains 'galleryRevision' -and
            $previousManifest.galleryVersion -eq $expectedBuild.galleryVersion -and
            $previousManifest.galleryRevision -eq $expectedBuild.galleryRevision) {
            Write-Host 'Qt 6 runtime already matches this Gallery build.'
            exit 0
        }
    }
}
$version = & (Join-Path $QtDir 'bin/qmake.exe') -query QT_VERSION
if ($LASTEXITCODE -ne 0 -or [version]$version -lt [version]'6.8' -or [version]$version -ge [version]'7.0') {
    throw 'A Qt 6.8+ Windows x64 MSVC SDK with Quick3D and QuickTimeline is required.'
}
$config = Get-Content (Join-Path $QtDir 'mkspecs/qconfig.pri') -Raw
if ($config -notmatch '(?m)^QT_ARCH\s*=\s*x86_64\s*$') { throw 'The runtime requires the x64 Qt SDK.' }
if (-not $CrtDir) {
    if ($env:VCToolsRedistDir) {
        $CrtDir = Get-ChildItem (Join-Path $env:VCToolsRedistDir 'x64') -Directory -Filter 'Microsoft.VC*.CRT' |
            Select-Object -First 1 -ExpandProperty FullName
    } elseif (-not $Refresh -or -not (Test-Path -LiteralPath (Join-Path $OutputDir 'vcruntime140.dll'))) {
        throw 'Use an x64 MSVC developer shell or specify -CrtDir.'
    }
}
$previousPath = $env:PATH
try {
    $env:PATH = "$QtDir/bin;$env:PATH"
    $binaryDir = Join-Path $BuildDir 'bin'
    Invoke-Checked $CMakeProgram @('-S', $repository, '-B', $BuildDir, '-G', 'Ninja',
        '-DCMAKE_BUILD_TYPE=Release', "-DCMAKE_PREFIX_PATH=$QtDir", "-DQT_DIR=$QtDir/lib/cmake/Qt6",
        "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$binaryDir", '-DEUI_BUILD_EXAMPLES=ON',
        '-DEUI_BUILD_TESTS=OFF', '-DQML_PLUGIN_DUMP:FILEPATH=OFF')
    $cache = Get-Content (Join-Path $BuildDir 'CMakeCache.txt') -Raw
    if ($cache -notmatch 'Qt6Quick3D_DIR:PATH=(?!.*NOTFOUND)' -or $cache -notmatch 'Qt6QuickTimeline_DIR:PATH=(?!.*NOTFOUND)') {
        throw 'Qt Quick 3D and QuickTimeline must be installed in this SDK.'
    }
    Invoke-Checked $CMakeProgram @('--build', $BuildDir, '--target', 'example', '--parallel', '4')
    $galleryBuild = Get-Content (Join-Path $BuildDir 'gallery-build.json') -Raw | ConvertFrom-Json
    if ($expectedBuild -and ($galleryBuild.galleryVersion -ne $expectedBuild.galleryVersion -or
        $galleryBuild.galleryRevision -ne $expectedBuild.galleryRevision)) {
        throw 'Gallery sources changed during the build. Build the main Gallery again to synchronize both programs.'
    }
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
    Copy-Item (Join-Path $binaryDir 'example.exe') (Join-Path $OutputDir 'EUI-Gallery-Qt6.exe')
    Copy-Item (Join-Path $binaryDir 'EUI') $OutputDir -Recurse -Force
    Invoke-Checked (Join-Path $QtDir 'bin/windeployqt.exe') @('--release', '--no-compiler-runtime',
        '--exclude-plugins', 'qtposition_nmea', '--qmldir', (Join-Path $repository 'example'), '--qmlimport', $binaryDir,
        (Join-Path $OutputDir 'EUI-Gallery-Qt6.exe'), (Join-Path $OutputDir 'EUI/euiplugin.dll'))
    if ($CrtDir) { Get-ChildItem -LiteralPath $CrtDir -Filter '*.dll' | Copy-Item -Destination $OutputDir }
    Set-Content (Join-Path $OutputDir 'qt.conf') -Encoding ascii -Value "[Paths]`nPrefix=.`nPlugins=.`nQmlImports=qml`nQml2Imports=qml`nTranslations=translations`nData=."
    $licenses = Join-Path $OutputDir 'licenses'
    New-Item -ItemType Directory -Path $licenses -Force | Out-Null
    Copy-Item (Join-Path $repository 'LICENSE') (Join-Path $licenses 'EUI-LICENSE.txt')
    Copy-Item (Join-Path $repository 'THIRD_PARTY_NOTICES.md') $licenses
    Copy-Item (Join-Path $PSScriptRoot 'licenses/Qt-*.txt') $licenses
    Copy-Item (Join-Path $PSScriptRoot 'licenses/FFmpeg-LGPL-2.1.txt') $licenses
    Copy-Item (Join-Path $repository 'example/quick3d/littlest-tokyo/assets/ATTRIBUTION.md') (Join-Path $licenses 'LittlestTokyo-ATTRIBUTION.md')
    Copy-Item (Join-Path $repository 'source/thirdparty/echarts/LICENSE') (Join-Path $licenses 'ECharts-LICENSE.txt')
    Copy-Item (Join-Path $repository 'source/thirdparty/echarts/NOTICE') (Join-Path $licenses 'ECharts-NOTICE.txt')
    Copy-Item (Join-Path $repository 'source/qt5/imports/eui/font/icons/LICENSE') (Join-Path $licenses 'Skill-Icons-LICENSE.txt')
    $qrSource = Get-Content (Join-Path $repository 'source/thirdparty/qrcodegen/qrcodegen.hpp') -Raw
    Set-Content (Join-Path $licenses 'QR-Code-generator-LICENSE.txt') -Encoding utf8 -Value ([regex]::Match($qrSource, '(?s)^/\*.*?\*/').Value)
    $files = @()
    $prefix = (Resolve-Path -LiteralPath $OutputDir).Path.TrimEnd('\', '/') + [IO.Path]::DirectorySeparatorChar
    foreach ($file in Get-ChildItem -LiteralPath $OutputDir -File -Recurse | Sort-Object FullName) {
        if ($file.FullName -eq $manifestPath) { continue }
        if ($file.Extension -in '.pdb', '.qmlc', '.jsc') { Remove-Item -LiteralPath $file.FullName; continue }
        $stream = [IO.File]::OpenRead($file.FullName)
        $algorithm = [Security.Cryptography.SHA256]::Create()
        try { $digest = [BitConverter]::ToString($algorithm.ComputeHash($stream)).Replace('-', '').ToLowerInvariant() }
        finally { $stream.Dispose(); $algorithm.Dispose() }
        $files += [ordered]@{ path = $file.FullName.Substring($prefix.Length).Replace('\', '/'); size = $file.Length;
            sha256 = $digest }
    }
    $manifest = [ordered]@{ schema = 1; architecture = 'x64'; qtVersion = $version;
        galleryVersion = $galleryBuild.galleryVersion; galleryRevision = $galleryBuild.galleryRevision;
        entry = 'EUI-Gallery-Qt6.exe'; files = $files }
    $temporaryManifest = Join-Path $OutputDir ('.manifest-' + [Guid]::NewGuid().ToString('N'))
    try {
        [IO.File]::WriteAllText($temporaryManifest, ($manifest | ConvertTo-Json -Depth 5), [Text.UTF8Encoding]::new($false))
        if (Test-Path -LiteralPath $manifestPath) { [IO.File]::Replace($temporaryManifest, $manifestPath, [NullString]::Value) }
        else { [IO.File]::Move($temporaryManifest, $manifestPath) }
    } finally {
        if (Test-Path -LiteralPath $temporaryManifest) { Remove-Item -LiteralPath $temporaryManifest }
    }
    Write-Host "Qt 6 runtime prepared: $OutputDir ($($files.Count) files)"
} finally {
    $env:PATH = $previousPath
}
