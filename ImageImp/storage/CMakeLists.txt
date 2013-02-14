include(${QT_USE_FILE})

link_directories(
	${base_link_dirs}
)

include_directories(
	${base_include_dirs}
)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

qt4_automoc(${sources})
QT4_WRAP_CPP(moc_sources ${headers})

add_library(storage ${sources} ${moc_sources})

#add_subdirectory(tests)

target_link_libraries(storage
)
