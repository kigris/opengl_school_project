#ifdef _WIN32 // If OS is Windows
#pragma warning(push, 0)
#elif __APPLE__ // If OS is MacOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#elif __linux__ // for Linux with Clang compiler
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#endif
// Warning of includes that it is wanted to be ignored
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifdef _WIN32 // If OS is Windows
#pragma warning(pop)
#else // If OS is MacOS or Linux
#pragma clang diagnostic pop
#endif

#include <iostream>
using std::cout, std::cerr, std::endl;

void onChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
    glViewport(0, 0, width, height);
    cout<<width<<" "<<height<<endl;
}

void handleInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char* argv[]) {
    if(!glfwInit()) {
        cerr<<"Error initializing GLFW"<<endl;
        return -1;
    }
    // Set the OpenGL version to 3.3 and Core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For MacOS forward-compatibility is needed
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
#endif
    
    // Create window and check if success
    GLFWwindow* window = glfwCreateWindow(800, 600, "60GIIN", nullptr, nullptr);
    if(!window) {
        cerr<<"Error creating window"<<endl;
        glfwTerminate();
        return -1;
    }
    // Set this window to the current context
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGL()) {
        cerr<<"Error initializing GLAD"<<endl;
        glfwTerminate();
        return -1;
    }
    
    // Set callback to 
    glfwSetFramebufferSizeCallback(window, onChangeFrameBufferSize);
    
    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        handleInput(window);
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}
