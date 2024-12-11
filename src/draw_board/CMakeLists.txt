

include_directories(${CMAKE_SOURCE_DIR}/src/draw_board
)


find_package(Qt5 COMPONENTS OpenGL REQUIRED)

set(DRAW_BOARD_SOURCES
	CDrawBoardWidget.cpp
	DrawWidget.cpp
	mypoint.cpp
	systemdata.cpp
	shape/contentedit.cpp
	shape/demensionsdata.cpp
	shape/ellipsedata.cpp
	shape/erasuredata.cpp
	shape/linedata.cpp
	shape/rectangledata.cpp
	shape/shapedata.cpp
	shape/textdata.cpp
	shape/triangeledata.cpp
	Resource/Res.qrc
)

add_library(draw_board STATIC ${DRAW_BOARD_SOURCES})


target_link_libraries(draw_board PRIVATE Qt5::OpenGL)