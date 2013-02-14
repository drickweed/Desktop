Application Installation Files

Contains folders for each application.
Each application folder contain subdirectories for different versions.
Each version folder contains the files required for installation

Minimum requirements for adding an application are creating a folder, a
version folder, and placing the installation files in the version folder.

Example: 7zip\4.57\7z457.msi

version folder naming convention:
* Version does not have to correspond to the actuall application version.
* May contain numbers, letters and dots. 
* May contain language suffix separated by "-"
* If a version is not specified in the installation request, the highest
  version in lexical order is selected.

Example: 5.2a-eng

Special files in application folder. The version for which the parameter is
intended can be specified as the extension (example: parameters.2.4b):

* reboot: if this file exists, the system will be rebooted after
        installation finishes.

* inst: installation method (msi, exe, copy, extract). If not present will be 
      guessed from installer filename.

* destination: In case of archive install method, extract to this destination.

* preinst: preinstallation commands

* postinst: post installation commands

* os: os specific version map. Contains lines of the format <os>,<version> to
    mark the only available version for certain os.

* runall: run all executables in a single version folder.

* parameters: command line parameters to pass to installer.
    See http:/www.appdeploy.com for a databse of silent installer command lines switches

* urls: url(s) for application download.

* dest: destination folder for 'copy' or 'extract' install methods.

* files: a folder containing extra installation files which do not come as part
    of the standard install for the application