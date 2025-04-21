set(ENABLE_TEST OFF CACHE BOOL "Disable tests" FORCE)
add_subdirectory(EventBus)

add_subdirectory(qwindowkit)

add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_subdirectory(spdlog)

set(YK_ENABLE_D3DTexture false)
add_subdirectory(cpp_base_lib)

add_subdirectory(whiteboard)

set(ENABLE_NETSSL ON CACHE BOOL "Enable NetSSL crypto engine")
add_subdirectory(poco)

add_subdirectory(sqlitecpp)