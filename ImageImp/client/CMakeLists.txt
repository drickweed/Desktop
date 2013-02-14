include(${QT_USE_FILE})

link_directories(
	${base_link_dirs}
	${EXIF_LIBRARY_DIRS}
)

include_directories(
	${base_include_dirs}
	${CMAKE_CURRENT_BINARY_DIR}/../widgets
	${EXIF_INCLUDE_DIRS}
)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# Windows application icon
if(WIN32)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.obj
	COMMAND c:/mingw/bin/windres.exe
	-I${CMAKE_CURRENT_SOURCE_DIR}
	-o ${CMAKE_CURRENT_BINARY_DIR}/icon.obj
	-i${CMAKE_CURRENT_SOURCE_DIR}/imageimp.rc
)
set(sources ${sources} ${CMAKE_CURRENT_BINARY_DIR}/icon.obj)
endif(WIN32)

qt4_automoc(${sources})

add_executable(imageimp WIN32 ${sources})

target_link_libraries(imageimp
	widgets
	document
	proxies
	model
	imp
	storage
	processor
	settings
	jb-qt4
	jb-qt4-widgets
	jb-std
	${QT_LIBRARIES}
	${QT_QTMAIN_LIBRARY}
	${EXIF_LIBRARIES}
)

INSTALL(TARGETS imageimp
    RUNTIME DESTINATION bin
)
