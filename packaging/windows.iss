#ifndef AppVersion
    #error AppVersion must be supplied by package-windows.ps1
#endif
#ifndef PackageDir
    #error PackageDir must be supplied by package-windows.ps1
#endif
#ifndef OutputDir
    #error OutputDir must be supplied by package-windows.ps1
#endif
#ifndef AppArchitecture
    #error AppArchitecture must be supplied by package-windows.ps1
#endif

[Setup]
AppId={{DF9088E7-852E-48D7-B4BB-B0E78E59B062}
AppName=EUI Gallery
AppVersion={#AppVersion}
AppPublisher=EUI
DefaultDirName={localappdata}\Programs\EUI Gallery
DefaultGroupName=EUI Gallery
PrivilegesRequired=lowest
#if AppArchitecture == "x64"
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
#else
ArchitecturesAllowed=x86compatible
#endif
MinVersion=10.0
DisableProgramGroupPage=yes
OutputDir={#OutputDir}
OutputBaseFilename=EUI-Gallery-{#AppVersion}-windows-{#AppArchitecture}-setup
SetupIconFile=..\assets\eui-icon.ico
UninstallDisplayIcon={app}\EUI-Gallery.exe
Compression=lzma2
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "chinesesimplified"; MessagesFile: "ChineseSimplified.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#PackageDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\EUI Gallery"; Filename: "{app}\EUI-Gallery.exe"; WorkingDir: "{app}"
Name: "{autodesktop}\EUI Gallery"; Filename: "{app}\EUI-Gallery.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\EUI-Gallery.exe"; Description: "{cm:LaunchProgram,EUI Gallery}"; Flags: nowait postinstall skipifsilent
