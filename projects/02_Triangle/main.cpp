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
#include <math.h>
#include "engine/window.hpp"
#include "engine/input.hpp"

void handleInput() {
}

//// Render VAO
//void render(uint32_t program, uint32_t VAO) {
//    glClear(GL_COLOR_BUFFER_BIT);
//    glUseProgram(program);
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//}


// // Render VBO
// void render(uint32_t program, uint32_t VAO, int n) {
//     glUseProgram(program);
//     glBindVertexArray(VAO);
//     glDrawArrays(GL_TRIANGLES, 0, n);
// }

// Render VAO
void render(uint32_t program, uint32_t VAO, int n) {
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, nullptr);
}

// 2 Triangles with 1 VBO and 1 VAO
uint32_t* createVertexData(uint32_t* VBO, uint32_t* EBO) {
    // Triangles /\ /\
    //
    float vertices[] = {
        // First triangle
        0.5f, 0.75f, 0.0f,
        0.1f, 0.0f, 0.0f,
        0.9f, 0.0f, 0.0f,
    };
    
    // Create vertex array object with 2 VAOs
    uint32_t* VAO = new uint32_t[2];
    glGenVertexArrays(1, &VAO[0]);
    glBindVertexArray(VAO[0]);
    
    // Create vertex buffer object
    glGenBuffers(1, &VBO[0]);

    // Bind the buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // Copy the vertex data to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Set attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    
    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO

    float verticesS[] = {
        // First triangle
       -0.1f, 0.0f, 0.0f,
       -0.5f, 0.75f, 0.0f,
       -0.9f, 0.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &VAO[1]);
    glBindVertexArray(VAO[1]);
    
    // Create vertex buffer object
    glGenBuffers(1, &VBO[1]);

    // Bind the buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // Copy the vertex data to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesS), verticesS, GL_STATIC_DRAW);
    
    // Set attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    
    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO

    // Return the VAO
    return VAO;
}


//// 2 Triangles with VAO, VBO and EBO
//uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {
//    // Triangles /\ /\
//    //
//    float vertices[] = {
//        // First triangle
//        0.5f, 0.75f, 0.0f,
//        0.1f, 0.0f, 0.0f,
//        0.9f, 0.0f, 0.0f,
//        -0.1f, 0.0f, 0.0f,
//        -0.5f, 0.75f, 0.0f,
//        -0.9f, 0.0f, 0.0f,
//    };
//
//    uint32_t indices[] = {
//        0, 1, 2,
//        3, 4, 5
//    };
//
//    // Create vertex buffer object
//    glGenBuffers(1, VBO);
//    glGenBuffers(1, EBO);
//
//    // Create vertex array object
//    uint32_t VAO;
//    glGenVertexArrays(1, &VAO);
//    // Bind the VAO
//    glBindVertexArray(VAO);
//
//    // Bind the buffer to the GL_ARRAY_BUFFER target
//    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
//    // Copy the vertex data to the buffer
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Bind the buffer to the GL_ELEMENT_ARRAY_BUFFER target
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Set attribute pointers
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
//    glEnableVertexAttribArray(0);
//
//    // Unbind the buffers
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    // Return the VAO
//    return VAO;
//}


//// Quad with 2 triangles
//uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {
//    float vertices[] = {
//        0.5f, 0.5f, 0.0f, // top right
//        0.5f, -0.5f, 0.0f, // bottom right
//        -0.5f, -0.5f, 0.0f, // bottom left
//        -0.5f,  0.5f, 0.0f // top left
//    };
//
//    uint32_t indices[] = { 0, 3, 1,  // first triangle
//                           1, 3, 2   // second triangle
//    };
//
//    // Create vertex buffer object
//    glGenBuffers(1, VBO);
//    glGenBuffers(1, EBO);
//
//    // Create vertex array object
//    uint32_t VAO;
//    glGenVertexArrays(1, &VAO);
//    // Bind the VAO
//    glBindVertexArray(VAO);
//
//    // Bind the buffer to the GL_ARRAY_BUFFER target
//    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
//    // Copy the vertex data to the buffer
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Bind the buffer to the GL_ELEMENT_ARRAY_BUFFER target
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Set attribute pointers
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
//    glEnableVertexAttribArray(0);
//
//    // Unbind the buffers
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    // Return the VAO
//    return VAO;
//}

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

//// Program for VAO, VBO and EBO
//uint32_t createProgram() {
//    // Vertex shader source code
//    const char* vertexShaderSource = "#version 330 core\n"
//    "layout (location=0) in vec3 aPos;\n"
//    "void main() {\n"
//    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//    "}\0";
//
//    // Fragment shader source code
//    const char* fragmentShaderSource = "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "void main() {\n"
//    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//    "}\0";
//
//    // Create vertex shader
//    const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    // Attach shader source code to shader object
//    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//    // Compile shader
//    glCompileShader(vertexShader);
//    // Check if compilation was successful
//    checkShader(vertexShader);
//
//    // Create fragment shader
//    const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    // Attach shader source code to shader object
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//    // Compile shader
//    glCompileShader(fragmentShader);
//    // Check if compilation was successful
//    checkShader(fragmentShader);
//
//    // Create shader program
//    const uint32_t program = glCreateProgram();
//    // Attach shaders to program
//    glAttachShader(program, vertexShader);
//    glAttachShader(program, fragmentShader);
//    // Link program
//    glLinkProgram(program);
//    // Check if linking was successful
//    checkProgram(program);
//
//    // Delete shaders
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Return program
//    return program;
//}

// Program for VBO
uint32_t createProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
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

// Fragment shader source code
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = vec4(0.2f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

uint32_t createHexagonData(uint32_t* VBO, uint32_t* EBO) {
    // Create vertex data
    float vertices[] = {
        0.2f, 0.7f, 0.0f,
        -0.2f, 0.7f, 0.0f,
        -0.7f, 0.0f, 0.0f,
        -0.2f, -0.7f, 0.0f,
        0.2f, -0.7f, 0.0f,
        0.7f, 0.0f, 0.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 4,
        4, 5, 0,
        0, 2, 4
    };

    uint32_t VAO;
    glGenBuffers(1, VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return VAO;
}

uint32_t createProceduralNSideObject(uint32_t* VBO, uint32_t* EBO, uint32_t* numVertices, int sides, float radius, float offX, float offY) {
    // Create vertex data
    float vertices[sides*3];
    uint32_t indices[((sides-2)*3)];
    *numVertices = (sides-2)*3;

    // Calculate angle between vertices in degrees
    float angle = 360.0f / sides;
    // Convert angle to radians
    float angleRad = angle * M_PI / 180.0f;

    // Create vertices
    for(int i=0; i < sides; i++) {
        vertices[i*3] = radius * cos(angleRad * i) + offX;
        vertices[i*3+1] = radius * sin(angleRad * i) + offY;
        vertices[i*3+2] = 0.0f;
    }

    // Create indices
    uint32_t curr_exp = 0;
    uint32_t counter = 0;
    uint32_t curr_pow = 0;
    uint32_t shapeSides = sides;
    while(shapeSides >= 3) {
        curr_pow = pow(2, curr_exp);
        for(int i=0; (curr_pow*(i+1)) < sides; i+=2) {
            if(i == 0)
                indices[counter] = 0;
            else
                indices[counter] = curr_pow*i;
            indices[counter+1] = (curr_pow*(i+1));
            if ((curr_pow*(i+2)) >= sides){
                indices[counter+2] = 0;
                counter += 3;
                break;
            }
            else
                indices[counter+2] = (curr_pow*(i+2));
            counter += 3;
        }
        shapeSides = ceil(shapeSides/2.0f);
        curr_exp++;
    }

    uint32_t VAO;
    glGenBuffers(1, VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return VAO;
}

int main() {
    // Get window instance
    Window* window = Window::instance();
    
    // Create program
    const uint32_t program = createProgram(vertexShaderSource, fragmentShaderSource);
    // const uint32_t program2 = createProgram(vertexShaderSource, fragmentShaderSource2);
    // uint32_t VBO[2], EBO[2];
    uint32_t VBO, EBO, numVertices;
    // Create vertex data
    // const uint32_t VAO = createVertexData(VBO, EBO);
    const uint32_t VAO = createProceduralNSideObject(&VBO, &EBO, &numVertices, 14, 0.5f, 0.4f, 0.2f);
    // Crea
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Display only lines
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Main loop
    while(window->isAlive()) {
        handleInput();
        glClear(GL_COLOR_BUFFER_BIT);
        render(program, VAO, numVertices);
        // render(program, VAO, 12);
        // render(program, VAO[0], 3);
        // render(program2, VAO[1], 3);
        window->frame();
    }

    // Delete program
    glDeleteProgram(program);
    // Delete vertex data
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteVertexArrays(1, &VAO[0]);
    // glDeleteVertexArrays(1, &VAO[1]);
    // glDeleteBuffers(1, &VBO[0]);
    // glDeleteBuffers(1, &VBO[1]);

    return 0;
}
