set(QT_USE_QTTEST TRUE)
include(${QT_USE_FILE})

link_directories(
	${base_link_dirs}
)

include_directories(
	${base_include_dirs}
)

FILE(GLOB test_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB test_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

qt4_automoc(${test_sources})
QT4_WRAP_CPP(moc_test_sources ${test_headers})

add_executable(test_model ${test_sources} ${moc_test_sources})

target_link_libraries(test_model
	imp
	model
	jb-qt4
	${QT_LIBRARIES}
	${QT_TEST_LIBRARY}
)

ADD_TEST(ModelTest ${EXECUTABLE_OUTPUT_PATH}/test_model)
