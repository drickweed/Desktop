include(${QT_USE_FILE})

link_directories(
	${base_library_dirs}
)

include_directories(
	${base_include_dirs}
)

SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --export-all-symbols")

QT4_ADD_RESOURCES(resource_files imageimp.qrc)

add_library(resources SHARED ${resource_files})

target_link_libraries(resources
	${QT_LIBRARIES}
)

INSTALL(TARGETS resources
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
)