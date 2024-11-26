
add_subdirectory(qwindowkit)

set(ENABLE_TEST OFF CACHE BOOL "Disable tests" FORCE)
add_subdirectory(EventBus)

add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_subdirectory(spdlog)

set(YK_ENABLE_D3DTexture false)
add_subdirectory(cpp_base_lib)