include(${QT_USE_FILE})

link_directories(
	${base_library_dirs}
	${EXIF_LIBRARY_DIRS}
	c:/build/lib
	c:/build/bin
	"C:/Program Files/GnuWin32/lib"
)

include_directories(
	${base_include_dirs}
	${CMAKE_CURRENT_BINARY_DIR}
	${EXIF_INCLUDE_DIRS}
	"C:/Program Files/GnuWin32/include"
)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

qt4_automoc(${sources})
QT4_WRAP_CPP(moc_sources ${headers})

SET(UIS cleaning.ui)
QT4_WRAP_UI(UIS_H ${UIS})

add_executable(imageimp_database_clean ${sources} ${moc_sources} ${UIS_H})

target_link_libraries(imageimp_database_clean
	processor
	settings
	jb-qt4
	jb-std
	raw
	popt
	${QT_LIBRARIES}
	${EXIF_LIBRARIES}
)

INSTALL(TARGETS imageimp_database_clean
    RUNTIME DESTINATION bin
)