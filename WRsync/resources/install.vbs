' Add dll paths for executables

Dim path, computer, rootKeyPath, key, intRC

Set fso = CreateObject("Scripting.FileSystemObject")
const HKEY_LOCAL_MACHINE = &H80000002
rootKeyPath = "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths" ' e.g. Software\JBox
computer = "." ' e.g. wks01 (use "." for local computer)
path = fso.GetParentFolderName(fso.GetParentFolderName(fso.GetFile(WScript.ScriptFullName))) & "\"


set objReg = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & computer & "\root\default:StdRegProv")
dllPath = path & "dlls"
binaries = Array("wrsettings", "wrsync", "logviewer", "wrmonitor")

for each exe in binaries
	key = rootKeyPath & "\" & exe & ".exe"
	intRC = objReg.CreateKey(HKEY_LOCAL_MACHINE, key)
	if intRC <> 0 then
		WScript.Echo "Error creating key: " & key & ". error(" & intRC & ")"
		WScript.Quit 1
	end if
	intRC = objReg.SetStringValue(HKEY_LOCAL_MACHINE, key, "Path", dllPath)
next

' Create shortcuts
Set shell = WScript.CreateObject("WScript.Shell")
allUsersProfile = shell.ExpandEnvironmentStrings("%AllUsersProfile%")
shortcutRoot = allUsersProfile & "\Start Menu\Programs\WR Backup\"

if not fso.FolderExists(shortcutRoot) then
	fso.CreateFolder(shortcutRoot)
end if
	
names = Array("Settings", "Run all jobs", "Log viewer", "Status monitor")

dim shortcut

for i=0 to Ubound(names)
	set shortcut = shell.CreateShortcut(shortcutRoot & names(i) & ".lnk")
	shortcut.Description = names(i)
	shortcut.TargetPath = path & binaries(i) & ".exe"
	shortcut.WorkingDirectory = path
	shortcut.save
next
