' Script to load photoshop script in windows.
' Allows us to run photoshop without knowning where it is actually installed.

Dim fso
Set fso = CreateObject("Scripting.FileSystemObject")

Dim scriptPath, parentPath

parentPath = fso.GetParentFolderName(fso.GetFile(WScript.ScriptFullName))
scriptPath = fso.BuildPath(parentPath, "import.jsx")

dim photoshop 
set photoshop = CreateObject("Photoshop.Application")
photoshop.DoJavaScriptFile(scriptPath)
photoshop.quit()
