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

add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Svg REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

if(${CMAKE_SIZEOF_VOID_P} STREQUAL "4")
	set(PLATFORM_WIN64 FALSE)
else()
	set(PLATFORM_WIN64 TRUE)
endif()

include_directories(${CMAKE_SOURCE_DIR}
					${CMAKE_SOURCE_DIR}/src
)

set(src_SOURCES
	src/public/base_page.cpp 
	src/public/yk_icon_button.cpp 
	src/public/yk_progress_bar.cpp 
	src/main.cpp
	src/main_window.cpp
	src/play_widget.cpp
	src/play_list_widget.cpp
	src/play_control_widget.cpp
	src/play_view.cpp
	src/vlc_wrapper/vlc_player.cpp
	ico.rc
	resources.qrc
)

set(unit_test_01_SOURCES
	unit_test/test_01.cpp
) 

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

if(MSVC)
	add_compile_options($<IF:$<CONFIG:Debug>,/MTd,/MT>)
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")
endif()

#add_compile_definitions(ASIO2_ENABLE_SSL ASIO2_ENABLE_LOG)

set(OPENSSL_ROOT_DIR ${CMAKE_SOURCE_DIR}/3rdparty/libopenssl/win64)

add_subdirectory(3rdparty)

include_directories(
				${OPENSSL_ROOT_DIR}/include
				#${CMAKE_SOURCE_DIR}/3rdparty/boost-1.81.0/libs/locale/include
				${CMAKE_SOURCE_DIR}/3rdparty/ElaWidgetTools/src/include
				${CMAKE_BINARY_DIR}
				${VLC_LIB_ROOT}/sdk/include
				)

link_directories(
		${OPENSSL_ROOT_DIR}/lib/VC/static
		${VLC_LIB_ROOT}/sdk/lib
)

#set(GENERATE_SETUP true)

if(${GENERATE_SETUP})
	#WIN32
	add_executable(${PROJECT_NAME} #WIN32
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

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network Qt6::Svg elawidgettools libvlc libvlccore)

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
