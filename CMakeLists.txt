cmake_minimum_required(VERSION 3.1.0)

include(env.cmake)
set(CMAKE_PREFIX_PATH ${QT_ROOT}/lib/cmake)

project(StarlightPlayer VERSION 1.0.1)

include(InstallRequiredSystemLibraries)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#if(MSVC)
#	add_compile_options($<IF:$<CONFIG:Debug>,/MTd,/MT>)
#	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")
#endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")  # /MD or /MDd
endif()

add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

add_definitions(-DSPDLOG_USE_STD_FORMAT)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Svg REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

if(${CMAKE_SIZEOF_VOID_P} STREQUAL "4")
	set(PLATFORM_WIN64 FALSE)
else()
	set(PLATFORM_WIN64 TRUE)
endif()

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")


add_subdirectory(3rdparty)

include_directories(${CMAKE_SOURCE_DIR}
					${CMAKE_SOURCE_DIR}/src
					${CMAKE_SOURCE_DIR}/3rdparty/EventBus/lib/src
					${CMAKE_SOURCE_DIR}/3rdparty
					${CMAKE_SOURCE_DIR}/3rdparty/asio2/include
					${CMAKE_SOURCE_DIR}/3rdparty/asio2/3rd
					${CMAKE_SOURCE_DIR}/src/widget
					${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/whiteboard/3rdparty/kk_qu_ui/src
					${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/whiteboard/src
)

set(src_SOURCES
	src/main.cpp
	src/context.cpp
	src/settings.cpp
	src/app_messages.cpp
	src/main_window.cpp
	src/widget/bg_page.cpp
	src/widget/play_widget.cpp
	src/widget/play_list_widget.cpp
	src/widget/play_control_widget.cpp
	src/widget/play_view.cpp
	src/widget/qdrag_widget.cpp
	src/widget/play_before_widget.cpp
	src/widget/play_list/media_list_widget.cpp
	src/widget/play_list/list_widget.cpp
	src/widget/play_list/list_func_page.cpp
	src/media_operation/media_operation.cpp
	src/media_operation/image_panel.cpp
	src/vlc_wrapper/vlc_player.cpp
	src/customize_io/file_get_base.cpp
    src/customize_io/file_get_http.cpp
    src/customize_io/file_get_http_impl.cpp
    src/customize_io/file_get_local.cpp
	ico.rc
	resources.qrc
	${CMAKE_SOURCE_DIR}/3rdparty/qwindowkit/examples/mainwindow/mainwindow.qrc
	${CMAKE_SOURCE_DIR}/3rdparty/qwindowkit/examples/shared/resources/shared.qrc
	${DRAW_BOARD_QRC}
)

message(DRAW_BOARD_QRC = ${DRAW_BOARD_QRC})

set(unit_test_01_SOURCES
	unit_test/test_01.cpp
) 


#add_compile_definitions(ASIO2_ENABLE_SSL ASIO2_ENABLE_LOG)

set(OPENSSL_ROOT_DIR ${CMAKE_SOURCE_DIR}/3rdparty/libopenssl/win64)



include_directories(
				${OPENSSL_ROOT_DIR}/include
				#${CMAKE_SOURCE_DIR}/3rdparty/boost-1.81.0/libs/locale/include
				${CMAKE_BINARY_DIR}
				${VLC_LIB_ROOT}/sdk/include
				${CMAKE_SOURCE_DIR}/3rdparty/qwindowkit/src/widgets
				${CMAKE_SOURCE_DIR}/3rdparty/qwindowkit/examples/shared/widgetframe
				)

link_directories(
		${OPENSSL_ROOT_DIR}/lib/VC/static
		${VLC_LIB_ROOT}/sdk/lib
		${CMAKE_SOURCE_DIR}/prebuild
)

set(GENERATE_SETUP false)

if(${GENERATE_SETUP})
	#WIN32
	add_executable(${PROJECT_NAME} WIN32
		${src_SOURCES}
	)
else()
	add_executable(${PROJECT_NAME}
		${src_SOURCES}
	)
endif()

#test



add_executable(test_01
		${unit_test_01_SOURCES}
)

target_link_libraries(test_01 libvlc libvlccore)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"")

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Network Qt5::Svg QWKCore QWKWidgets WidgetFrame draw_board libvlc libvlccore yk_base_lib 
	Poco::Foundation
    Poco::Util
    Poco::Net
	Poco::NetSSL
)

configure_file(${VLC_LIB_ROOT}/libvlc.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libvlc.dll COPYONLY)
configure_file(${VLC_LIB_ROOT}/npvlc.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/npvlc.dll COPYONLY)
configure_file(${VLC_LIB_ROOT}/libvlccore.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libvlccore.dll COPYONLY)
configure_file(${VLC_LIB_ROOT}/axvlc.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/axvlc.dll COPYONLY)

file(COPY ${VLC_LIB_ROOT}/hrtfs DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${VLC_LIB_ROOT}/languages DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${VLC_LIB_ROOT}/locale DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${VLC_LIB_ROOT}/lua DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${VLC_LIB_ROOT}/msi DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${VLC_LIB_ROOT}/plugins DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${VLC_LIB_ROOT}/sdk DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${VLC_LIB_ROOT}/skins DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_custom_command(
					TARGET ${PROJECT_NAME}
					POST_BUILD
					COMMAND ${QT_ROOT}/bin/windeployqt.exe --debug ${PROJECT_NAME}.exe
					WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	)
else()
	add_custom_command(
					TARGET ${PROJECT_NAME}
					POST_BUILD
					COMMAND ${QT_ROOT}/bin/windeployqt.exe --release ${PROJECT_NAME}.exe
					WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	)
endif()
