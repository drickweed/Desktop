cmake_minimum_required(VERSION 2.6)
project(IMAGEIMP)

IF(UNIX)
SET(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
ENDIF(UNIX)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -DQT_NO_DEBUG_OUTPUT")
#set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -DQT_NO_DEBUG_OUTPUT")

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")

find_package(Qt4 REQUIRED)
find_package(PkgConfig)
#pkg_check_modules(SQLITE sqlite3)
#pkg_check_modules(XMLMM libxml++-2.6)
pkg_check_modules(JBSTD jb-std)
pkg_check_modules(JBEXIF jb-exif)
#pkg_check_modules(JBBOOST jb-boost)
pkg_check_modules(JBQT jb-qt4)
pkg_check_modules(EXIF libexif)

set(LIBRAW_LIBRARIES raw)

IF(WIN32)
set(JB_LIBRARY_DIRS "c:/build/bin")
set(JB_INCLUDE_DIRS "c:/build/include")
#set(BOOST_INCLUDE_DIRS c:/mingw/boost)
#set(BOOST_LIBRARY_DIRS c:/mingw/boost/stage/lib)
set(EXIF_LIBRARIES exif)
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-mwindows -DLIBRAW_NODLL")
set(CMAKE_INSTALL_PREFIX "c:/releases/imageimp")
set(LIBRAW_NODLL 1)
#set(LIBRAW_LIBRARIES raw ws2_32 m)
ENDIF(WIN32)

set(base_include_dirs
	${JB_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS}
	${IMAGEIMP_SOURCE_DIR}
)

set(base_link_dirs
	${JB_LIBRARY_DIRS}
	${QT_LIBRARY_DIRS}
	c:/build/lib
	c:/build/bin
)

ENABLE_TESTING()

set(QT_USE_QTSQL True)

add_subdirectory(imp)
add_subdirectory(model)
add_subdirectory(config)
add_subdirectory(model_proxies)
add_subdirectory(document)
add_subdirectory(settings)
add_subdirectory(widgets)
add_subdirectory(client)
add_subdirectory(processor)
add_subdirectory(storage)
#add_subdirectory(resources)
#add_subdirectory(dbcleaner)
