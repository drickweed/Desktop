!win32:PREFIX="/usr/local"
win32:PREFIX="c:\work\build\plugins"

prefix.path = $$PREFIX

!win32:prefix.bin = $$PREFIX/bin
win32:prefix.bin = $$PREFIX

unix {
	sharedlibs.files = lib$${TARGET}.so*
	sharedlibs.files = lib$${TARGET}.a
	sharedlibs.path = $$prefix.path/lib
	INSTALLS += sharedlibs
}

win32 {
	dll.files = $${TARGET}.dll
	dll.path = $$prefix.bin
	INSTALLS += dll

	exe.files = $${TARGET}.exe
	exe.path = $$prefix.bin
	INSTALLS += exe
}
