#Include paths for deps
set(INCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/stb)

#Common Dependency Files
set(DEPS_SRC_FILES_COMMON
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/monitor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/window.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/vulkan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glad/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/stb/stb_image.h)

#Windows Dependency Files
set(DEPS_SRC_FILES_WIN
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/egl_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_joystick.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_joystick.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_monitor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/win32_window.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/wgl_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/osmesa_context.c)

#Linux Dependency Files
set(DEPS_SRC_FILES_LINUX
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/glx_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/x11_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/x11_monitor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/x11_platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/x11_window.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/egl_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/posix_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/posix_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/linux_joystick.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/xkb_unicode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/osmesa_context.c)

#MacOS Dependency Files
set(DEPS_SRC_FILES_MACOS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/cocoa_init.m
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/cocoa_joystick.m
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/cocoa_monitor.m
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/cocoa_window.m
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/nsgl_context.m
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/posix_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/cocoa_joystick.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/cocoa_platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/cocoa_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/egl_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deps/glfw/osmesa_context.c)

#Remove warnings for deps files
if (APPLE)
    set_source_files_properties(
        ${DEPS_SRC_FILES_COMMON}
        ${DEPS_SRC_FILES_WIN}
        ${DEPS_SRC_FILES_LINUX}
        ${DEPS_SRC_FILES_MACOS}
        PROPERTIES
        COMPILE_FLAGS "-Wall"
    )
else()
    set_source_files_properties(
        ${DEPS_SRC_FILES_COMMON}
        ${DEPS_SRC_FILES_WIN}
        ${DEPS_SRC_FILES_LINUX}
        ${DEPS_SRC_FILES_MACOS}
        PROPERTIES
        COMPILE_FLAGS "-Wall"
    )
endif()
