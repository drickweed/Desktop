' Remove dll paths for executables

Dim path, computer, rootKeyPath, key, intRC

Set fso = CreateObject("Scripting.FileSystemObject")
const HKEY_LOCAL_MACHINE = &H80000002
rootKeyPath = "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths" ' e.g. Software\Rallencorp
computer = "." ' e.g. wks01 (use "." for local computer)
path = fso.GetParentFolderName(fso.GetParentFolderName(fso.GetFile(WScript.ScriptFullName))) & "\dlls"
set objReg = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & computer & "\root\default:StdRegProv")

binaries  = Array("control", "groupedit", "grouporder", "photomatch", "photoimport")

for each exe in binaries
	key = rootKeyPath & "\" & exe & ".exe"
	intRC = objReg.DeleteKey(HKEY_LOCAL_MACHINE, key)
	if intRC <> 0 then
		WScript.Echo "Error removing key: " & key & ". error(" & intRC & ")"
		WScript.Quit 1
	end if
next

' Delete shortcuts
Set shell = WScript.CreateObject("WScript.Shell")
allUsersProfile = shell.ExpandEnvironmentStrings("%AllUsersProfile%")
shortcutRoot = allUsersProfile & "\Start Menu\Programs\ImageImp\"

if fso.FolderExists(shortcutRoot) then
	set folder = fso.getFolder(shortcutRoot)
	folder.Delete
end if
