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

qt4_automoc(${sources})
QT4_WRAP_CPP(moc_sources ${headers})

SET(IMAGEIMP_UIS imageimp.ui)
QT4_WRAP_UI(IMAGEIMP_UIS_H ${IMAGEIMP_UIS})

add_library(widgets ${sources} ${moc_sources} ${IMAGEIMP_UIS} ${IMAGEIMP_UIS_H} )

target_link_libraries(widgets
)

