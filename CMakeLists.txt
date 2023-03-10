cmake_minimum_required(VERSION 3.25)

project(60GIIN CXX C)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config types" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Files for dependencies and platform specific config
include(cmake/deps_files.cmake)
include(cmake/platform.cmake)
include(cmake/compiler.cmake)

#****************************************************
# Engine Library
#****************************************************
# Engine Source Files
include(cmake/engine_files.cmake)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/ PREFIX "Source files" FILES ${ENGINE_SRC_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/engine PREFIX "Header files" FILES ${ENGINE_HEADER_FILES})

add_library(EngineLib STATIC ${ENGINE_SRC_FILES} ${ENGINE_HEADER_FILES} ${DEPS_SRC_FILES_COMMON})

target_include_directories(EngineLib PRIVATE ${INCLUDE_PATHS})
target_compile_definitions(EngineLib PRIVATE -D_GLFW_USE_OPENGL=1)
target_compile_features(EngineLib PRIVATE cxx_std_17)

configure_platform(EngineLib)
configure_compiler(EngineLib)

#****************************************************
# Projects
#****************************************************
include(cmake/projects.cmake)

# Root path of the projects
set(PROJECTS_PATH ${CMAKE_CURRENT_LIST_DIR}/projects)
# Configure all projects
foreach(PROJECT ${PROJECT_LIST})
    message(STATUS "Configuring project:" ${PROJECT})
    # Create project folder
    file(MAKE_DIRECTORY ${PROJECTS_PATH}/${PROJECT})
    # Create main.cpp file
    file(TOUCH ${PROJECTS_PATH}/${PROJECT}/main.cpp)

    # Create project and add configure
    add_executable(${PROJECT} ${PROJECTS_PATH}/${PROJECT}/main.cpp ${DEPS_SRC_FILES})
    target_include_directories(${PROJECT} PRIVATE ${INCLUDE_PATHS})
    target_compile_definitions(${PROJECT} PRIVATE -D_GLFW_USE_OPENGL=1)
    target_link_libraries(${PROJECT} PRIVATE EngineLib)
    target_compile_features(${PROJECT} PRIVATE cxx_std_17)
    configure_platform(${PROJECT})
    configure_compiler(${PROJECT})

    # Shaders
    file(GLOB SHADERS_LIST ${PROJECTS_PATH}/${PROJECT}/*.vs ${PROJECTS_PATH}/${PROJECT}/.fs)
    target_sources(${PROJECT} PRIVATE ${SHADERS_LIST})
    source_group(Shaders FILES ${SHADER_LIST})
endforeach()
