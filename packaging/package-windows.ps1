[CmdletBinding()]
param(
    [string]$QtDir,
    [string]$IsccPath,
    [string]$OpenSslDir
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Invoke-Checked {
    param([string]$Program, [string[]]$Arguments)

    & $Program @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "$Program failed with exit code $LASTEXITCODE"
    }
}

$euiRoot = Split-Path $PSScriptRoot -Parent
$buildDir = Join-Path $euiRoot 'build\windows-package'
$outputDir = Join-Path $euiRoot 'dist'
$packageDir = Join-Path $outputDir 'EUI-Gallery'
$versionMatch = [regex]::Match((Get-Content (Join-Path $euiRoot 'CMakeLists.txt') -Raw), 'project\(EUI VERSION ([\d.]+)\)')
if (-not $versionMatch.Success) {
    throw 'Cannot read the EUI version from CMakeLists.txt.'
}
$appVersion = $versionMatch.Groups[1].Value
if ($appVersion.Split('.').Count -eq 2) {
    $appVersion += '.0'
}

foreach ($tool in 'cmake.exe', 'ninja.exe', 'cl.exe') {
    if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) {
        throw "Missing $tool. Run this script in an x64 Visual Studio Developer PowerShell with CMake and Ninja on PATH."
    }
}
if ($env:VSCMD_ARG_TGT_ARCH -ne 'x64') {
    throw 'The Visual Studio build environment must target x64.'
}
if (-not $QtDir) {
    $qmake = Get-Command qmake.exe -ErrorAction Stop
    $QtDir = Split-Path (Split-Path $qmake.Source -Parent) -Parent
}
$QtDir = (Resolve-Path -LiteralPath $QtDir).Path
$deployTool = Join-Path $QtDir 'bin\windeployqt.exe'
$qtVersion = & (Join-Path $QtDir 'bin\qmake.exe') -query QT_VERSION
if ($LASTEXITCODE -ne 0 -or $qtVersion -notmatch '^5\.15\.') {
    throw 'This packaging script requires a Qt 5.15 MSVC x64 Kit.'
}
$qtSpec = & (Join-Path $QtDir 'bin\qmake.exe') -query QMAKE_XSPEC
$qtConfig = Get-Content (Join-Path $QtDir 'mkspecs\qconfig.pri') -Raw
if ($LASTEXITCODE -ne 0 -or $qtSpec -ne 'win32-msvc' -or $qtConfig -notmatch '(?m)^QT_ARCH\s*=\s*x86_64\s*$') {
    throw 'The Qt Kit must use MSVC and target x64.'
}
if (-not $IsccPath) {
    $iscc = Get-Command ISCC.exe -ErrorAction SilentlyContinue
    if ($iscc) {
        $IsccPath = $iscc.Source
    } else {
        $IsccPath = @(
            (Join-Path $euiRoot 'build\packaging-tools\inno\ISCC.exe'),
            "${env:ProgramFiles(x86)}\Inno Setup 6\ISCC.exe",
            "$env:LOCALAPPDATA\Programs\Inno Setup 6\ISCC.exe"
        ) | Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } | Select-Object -First 1
    }
}
if (-not $IsccPath -or -not (Test-Path -LiteralPath $IsccPath -PathType Leaf)) {
    throw 'Inno Setup 6.5 or newer is required. Supply -IsccPath pointing to ISCC.exe.'
}
if (-not $OpenSslDir) {
    $OpenSslDir = @(
        (Join-Path $QtDir 'bin'),
        (Join-Path (Split-Path (Split-Path $QtDir -Parent) -Parent) 'Tools\OpenSSL\Win_x64\bin'),
        (Join-Path $euiRoot 'build\packaging-tools\openssl\openssl-1.1\x64\bin')
    ) | Where-Object { Test-Path -LiteralPath (Join-Path $_ 'libssl-1_1-x64.dll') } | Select-Object -First 1
}
foreach ($library in 'libssl-1_1-x64.dll', 'libcrypto-1_1-x64.dll') {
    if (-not $OpenSslDir -or -not (Test-Path -LiteralPath (Join-Path $OpenSslDir $library) -PathType Leaf)) {
        throw "Missing $library. Supply -OpenSslDir containing the OpenSSL 1.1.1 x64 runtime DLLs."
    }
}
if (-not $env:VCToolsRedistDir) {
    throw 'VCToolsRedistDir is missing. Initialize the Visual Studio Developer PowerShell first.'
}
$crtDir = Get-ChildItem -LiteralPath (Join-Path $env:VCToolsRedistDir 'x64') -Directory -Filter 'Microsoft.VC*.CRT' |
    Select-Object -First 1 -ExpandProperty FullName
if (-not $crtDir) {
    throw 'Cannot locate the x64 MSVC redistributable DLLs.'
}

$originalPath = $env:PATH
try {
    $env:PATH = "$QtDir\bin;$env:PATH"
    Invoke-Checked 'cmake.exe' @(
        '-S', $euiRoot, '-B', $buildDir, '-G', 'Ninja',
        '-DCMAKE_BUILD_TYPE=Release', "-DCMAKE_PREFIX_PATH=$QtDir",
        "-DQt5_DIR=$QtDir/lib/cmake/Qt5", '-DEUI_BUILD_EXAMPLES=ON',
        '-DEUI_BUILD_STATIC_LIB=OFF', '-DEUI_BUILD_TESTS=OFF',
        '-DQML_PLUGIN_DUMP:FILEPATH=OFF'
    )
    Invoke-Checked 'cmake.exe' @('--build', $buildDir, '--target', 'example', '--parallel')

    New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
    if ((Get-Item -LiteralPath $outputDir).Attributes -band [IO.FileAttributes]::ReparsePoint) {
        throw 'The dist directory must not be a symbolic link or junction.'
    }
    if (Test-Path -LiteralPath $packageDir) {
        $resolvedPackage = (Resolve-Path -LiteralPath $packageDir).Path
        $expectedPackage = Join-Path (Resolve-Path -LiteralPath $euiRoot).Path 'dist\EUI-Gallery'
        if ($resolvedPackage -ne $expectedPackage -or
            (Get-Item -LiteralPath $packageDir).Attributes -band [IO.FileAttributes]::ReparsePoint) {
            throw "Refusing to clean an unexpected package directory: $resolvedPackage"
        }
        Remove-Item -LiteralPath $resolvedPackage -Recurse -Force
    }
    New-Item -ItemType Directory -Path $packageDir | Out-Null
    Copy-Item -LiteralPath (Join-Path $euiRoot 'bin\Release\example.exe') -Destination (Join-Path $packageDir 'EUI-Gallery.exe')
    Copy-Item -LiteralPath (Join-Path $euiRoot 'source\Qt5\imports\EUI') -Destination $packageDir -Recurse
    Copy-Item -LiteralPath (Join-Path $buildDir 'source\euiplugin.dll') -Destination (Join-Path $packageDir 'EUI')
    Invoke-Checked $deployTool @(
        '--release', '--no-compiler-runtime', '--no-virtualkeyboard',
        '--qmldir', (Join-Path $euiRoot 'example\qml'), '--qmlimport', $packageDir,
        '--svg', '--multimedia', (Join-Path $packageDir 'EUI-Gallery.exe'),
        (Join-Path $packageDir 'EUI\euiplugin.dll')
    )
    Get-ChildItem -LiteralPath $crtDir -Filter '*.dll' | Copy-Item -Destination $packageDir
    foreach ($library in 'libssl-1_1-x64.dll', 'libcrypto-1_1-x64.dll') {
        Copy-Item -LiteralPath (Join-Path $OpenSslDir $library) -Destination $packageDir
    }
    Set-Content -LiteralPath (Join-Path $packageDir 'qt.conf') -Encoding ASCII -Value "[Paths]`nPrefix=.`nPlugins=.`nQml2Imports=.`nTranslations=translations`nData=."

    $licensesDir = Join-Path $packageDir 'licenses'
    New-Item -ItemType Directory -Path $licensesDir | Out-Null
    Copy-Item -LiteralPath (Join-Path $euiRoot 'LICENSE') -Destination (Join-Path $licensesDir 'EUI-LICENSE.txt')
    Copy-Item -LiteralPath (Join-Path $euiRoot 'THIRD_PARTY_NOTICES.md') -Destination $licensesDir
    Copy-Item -Path (Join-Path $PSScriptRoot 'licenses\*') -Destination $licensesDir
    Copy-Item -LiteralPath (Join-Path $euiRoot 'source\Qt5\imports\EUI\Font\Icons\LICENSE') -Destination (Join-Path $licensesDir 'Skill-Icons-LICENSE.txt')
    Copy-Item -LiteralPath (Join-Path $euiRoot 'source\ThirdParty\echarts\LICENSE') -Destination (Join-Path $licensesDir 'ECharts-LICENSE.txt')
    Copy-Item -LiteralPath (Join-Path $euiRoot 'source\ThirdParty\echarts\NOTICE') -Destination (Join-Path $licensesDir 'ECharts-NOTICE.txt')
    $qrSource = Get-Content (Join-Path $euiRoot 'source\ThirdParty\qrcodegen\qrcodegen.hpp') -Raw
    Set-Content -LiteralPath (Join-Path $licensesDir 'QR-Code-generator-LICENSE.txt') -Encoding UTF8 -Value ([regex]::Match($qrSource, '(?s)^/\*.*?\*/').Value)
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'README.txt') -Destination $packageDir

    $archive = Join-Path $outputDir "EUI-Gallery-$appVersion-windows-x64-portable.zip"
    Compress-Archive -LiteralPath $packageDir -DestinationPath $archive -CompressionLevel Optimal -Force
    Invoke-Checked $IsccPath @(
        "/DAppVersion=$appVersion", "/DPackageDir=$packageDir", "/DOutputDir=$outputDir",
        (Join-Path $PSScriptRoot 'windows.iss')
    )
    Get-ChildItem -LiteralPath $outputDir -File | Where-Object Name -like "EUI-Gallery-$appVersion-windows-x64-*" |
        ForEach-Object { '{0}  {1}' -f (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash, $_.Name } |
        Set-Content -LiteralPath (Join-Path $outputDir 'SHA256SUMS.txt') -Encoding ASCII
    Write-Host "Packages created in $outputDir"
} finally {
    $env:PATH = $originalPath
}
