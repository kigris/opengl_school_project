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
#ifdef _WIN32 // If OS is Windows
#pragma warning(pop)
#else // If OS is MacOS or Linux
#pragma clang diagnostic pop
#endif

#include <iostream>
#include "engine/window.hpp"
#include "engine/input.hpp"

void handleInput() {
}

void render(uint32_t program, uint32_t VAO) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {
    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f // top left
    };
    
    uint32_t indices[] = { 0, 3, 1,  // first triangle
                           1, 3, 2   // second triangle
    };

    // Create vertex buffer object
    glGenBuffers(1, VBO);
    // Create vertex array object
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    // Copy the vertex data to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the buffer to the GL_ELEMENT_ARRAY_BUFFER target
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Set attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Return the VAO
    return VAO;
}

bool checkShader(const uint32_t shader) {
    int32_t success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr<<"Error compiling shader: "<<infoLog<<std::endl;
        return false;
    }
    return true;
}

bool checkProgram(const uint32_t program) {
    int32_t success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr<<"Error linking program: "<<infoLog<<std::endl;
        return false;
    }
    return true;
}

uint32_t createProgram() {
    // Vertex shader source code
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location=0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    // Fragment shader source code
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

    // Create vertex shader
    const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach shader source code to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    // Compile shader
    glCompileShader(vertexShader);
    // Check if compilation was successful
    checkShader(vertexShader);

    // Create fragment shader
    const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach shader source code to shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    // Compile shader
    glCompileShader(fragmentShader);
    // Check if compilation was successful
    checkShader(fragmentShader);

    // Create shader program
    const uint32_t program = glCreateProgram();
    // Attach shaders to program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    // Link program
    glLinkProgram(program);
    // Check if linking was successful
    checkProgram(program);

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Return program
    return program;
}

int main() {
    // Get window instance
    Window* window = Window::instance();

    // Create program
    const uint32_t program = createProgram();
    uint32_t VBO, EBO;
    // Create vertex data
    const uint32_t VAO = createVertexData(&VBO, &EBO);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Display only lines
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Main loop
    while(window->isAlive()) {
        handleInput();
        render(program, VAO);
        window->frame();
    }

    // Delete program
    glDeleteProgram(program);
    // Delete vertex data
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}
