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
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifdef _WIN32 // If OS is Windows
#pragma warning(pop)
#else // If OS is MacOS or Linux
#pragma clang diagnostic pop
#endif

#include <iostream>
#include "engine/window.hpp"
#include "engine/shader.hpp"

// Render VAO
void render(const Shader &shader, uint32_t VAO, int n, uint32_t tex, uint32_t tex2)
{
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);

    shader.set("texture1", 0);
    shader.set("texture2", 1);

    glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, nullptr);
}

// Quad with 2 triangles
uint32_t createVertexData(uint32_t *VBO, uint32_t *EBO)
{
    float vertices[] = {
        // positions         // colors             // texture coords
        0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,     0.0f, 1.0f
    };

    uint32_t indices[] = {
        0, 3, 1, // first triangle
        1, 3, 2  // second triangle
        
    };

    // Create vertex buffer object
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Return the VAO
    return VAO;
}

uint32_t createTexture(const char* path) {
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if(!data) {
        std::cout << "Failed to load texture" << std::endl;
        exit(EXIT_FAILURE);
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}

int main()
{
    // Get window instance
    Window *window = Window::instance();

    // Create program
    const Shader shader("../../projects/04_Textures/vertex.vert", "../../projects/04_Textures/fragment.frag");
    uint32_t VBO, EBO;
    // uint32_t VBO, EBO, numVertices;
    // Create vertex data
    uint32_t VAO = createVertexData(&VBO, &EBO);
    // const uint32_t VAO = createProceduralNSideObject(&VBO, &EBO, &numVertices, 99, 0.5f, 0.4f, 0.2f);
    // Crea
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Display only lines
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    uint32_t tex = createTexture("../../assets/textures/blue_blocks.jpg");
    uint32_t tex2 = createTexture("../../assets/textures/bricks_arrow.jpg");

    // Main loop
    while (window->isAlive())
    {
        // handleInput();
        // render(program, VAO, numVertices);
        render(shader, VAO, 6, tex, tex2);
        // render(program, VAO[0], 3);
        // render(program2, VAO[1], 3);
        window->frame();
    }

    // Delete vertex data
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glDeleteVertexArrays(1, &VAO[0]);
    // glDeleteVertexArrays(1, &VAO[1]);
    // glDeleteBuffers(1, &VBO[0]);
    // glDeleteBuffers(1, &VBO[1]);

    return 0;
}
