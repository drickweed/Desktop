include(${QT_USE_FILE})

link_directories(
	${base_link_dirs}
)

include_directories(
	${base_include_dirs}
	${CMAKE_CURRENT_BINARY_DIR}
)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Windows application icon
if(WIN32)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.obj
	COMMAND c:/mingw/bin/windres.exe
	-I${CMAKE_CURRENT_SOURCE_DIR}
	-o ${CMAKE_CURRENT_BINARY_DIR}/icon.obj
	-i${CMAKE_CURRENT_SOURCE_DIR}/config.rc
)
set(sources ${sources} ${CMAKE_CURRENT_BINARY_DIR}/icon.obj)
endif(WIN32)

qt4_automoc(${sources})
QT4_WRAP_CPP(moc_sources ${headers})

SET(CONFIG_UIS config.ui)
QT4_WRAP_UI(CONFIG_UIS_H ${CONFIG_UIS})

qt4_add_resources(resources config.qrc)

add_executable(config WIN32 ${sources} ${resources} ${moc_sources} ${CONFIG_UIS} ${CONFIG_UIS_H} ${headers})

target_link_libraries(config
	settings
	jb-qt4
	jb-std
	${QT_LIBRARIES}
	${QT_QTMAIN_LIBRARY}
)

INSTALL(TARGETS config
    RUNTIME DESTINATION bin
)

