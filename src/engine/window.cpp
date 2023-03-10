#include "engine/window.hpp"
#include "engine/input.hpp"

#ifdef _WIN32 // If OS is Windows
#pragma warning(push, 0) // Disable warnings
#elif __APPLE__ // If OS is MacOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#elif __linux__ // If OS is Linux with Clang compiler
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#endif
// Warnings from includes to be ignored
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifdef _WIN32 // If OS is Windows
#pragma warning(pop) // Restore warnings
#elif __APPLE__ // If OS is MacOS
#pragma clang diagnostic pop // Restore warnings
#elif __linux__ // If OS is Linux with Clang compiler
#pragma clang diagnostic pop // Restore warnings
#endif

#include <iostream>

void onChangeFrameBufferSize(GLFWwindow*, const int32_t width, const int32_t height) noexcept {
    // Static cast to uint32_t because OpenGL doesn't accept negative values
    Window::instance()->setWidth(static_cast<uint32_t>(width));
    Window::instance()->setHeight(static_cast<uint32_t>(height));
    // Set the viewport to the new size
    glViewport(0, 0, width, height);
    // Display the new size
    std::cout << "Window size: " << width << "x" << height << std::endl;
}

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else {
        Input::instance()->keyPressed(key, action);
    }
}

void onMouse(GLFWwindow*, double xpos, double ypos) {
    Input::instance()->mouseMoved(static_cast<float>(xpos), static_cast<float>(ypos));
}

void onScroll(GLFWwindow*, double xoffset, double yoffset) {
    Input::instance()->mouseScrolled(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

Window* Window::instance() {
    // Static initialization
    static Window w;
    return &w;
}

Window::Window() {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Set the OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set the OpenGL profile to core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For MacOS forward-compatibility is needed and retina frambuffers is enabled
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif

    _window = glfwCreateWindow(static_cast<int>(_width), static_cast<int>(_height), "60GIIN", nullptr, nullptr);
    if (!_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set current context to the window
    glfwMakeContextCurrent(_window);

    // Set the frame buffer size callback
    glfwSetFramebufferSizeCallback(_window, onChangeFrameBufferSize);

    // Set other callbacks
    glfwSetKeyCallback(_window, onKeyPressed);
    glfwSetCursorPosCallback(_window, onMouse);
    glfwSetScrollCallback(_window, onScroll);

    if(!gladLoadGL()) {
        std::cerr<<"Failed to initialize GLAD"<<std::endl;
        exit(EXIT_FAILURE);
    }
    setCaptureMouse(true);
}

// Destructor
Window::~Window() {
    glfwTerminate();
}

// Function to check if the window is alive
bool Window::isAlive() const {
    return !glfwWindowShouldClose(_window);
}

void Window::frame() const {
    // Swap the buffers
    glfwSwapBuffers(_window);
    // Poll for events
    glfwPollEvents();
}

bool Window::isKeyPressed(int key) {
    return glfwGetKey(Window::instance()->getNativeWindow(), key) == GLFW_PRESS;
}

void Window::setCaptureMouse(bool toggle) const {
    glfwSetInputMode(_window, GLFW_CURSOR, toggle ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
