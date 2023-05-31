set(ENGINE_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/input.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/shader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/texture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/mesh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/model.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/light.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/geometry/geometry.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/geometry/quad.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/geometry/cube.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/geometry/sphere.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/geometry/teapot.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/geometry/teapotdata.hpp
)
set(ENGINE_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/deps.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/geometry/geometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/geometry/quad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/geometry/cube.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/geometry/sphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/geometry/teapot.cpp
)