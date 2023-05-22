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
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifdef _WIN32 // If OS is Windows
#pragma warning(pop)
#else // If OS is MacOS or Linux
#pragma clang diagnostic pop
#endif

#include <iostream>
#include <vector>
#include "engine/window.hpp"
#include "engine/shader.hpp"

// Render VAO
void render(const Shader &shader, uint32_t VAO, int n, uint32_t tex, uint32_t tex2)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);

    shader.set("texture1", 0);
    shader.set("texture2", 1);

    // glm::mat4 transform = glm::mat4(1.0f);
    // transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));
    // transform = glm::scale(transform, glm::vec3(0.5f,0.5f,0.5f));
    // shader.set("transform", transform);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 0.5f, 0.5f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    const glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    // float aspect = 800.0f / 600.0f;
    // const glm::mat4 proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.1f, 100.0f);
    // const glm::mat proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f)
    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    std::vector<glm::vec3> cubePositions{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 6.0f, -20.0f),
        glm::vec3(-2.0f, -2.0f, -2.0f),
        glm::vec3(-4.0f, -6.0f, -10.0f),
        glm::vec3(3.0f, -1.0f, -4.0f),
        glm::vec3(-2.0f, 3.0f, -8.0f),
        glm::vec3(2.0f, -2.0f, -3.0f),
        glm::vec3(1.0f, 2.0f, -2.0f),
        glm::vec3(1.0f, 0.0f, -2.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),
        glm::vec3(-4.0f, 1.0f, -1.0f),
        glm::vec3(-4.0f, 1.0f, -2.0f)
        };

    for (size_t i = 0; i < cubePositions.size(); ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        const float angle = 10.0f + (20.0f * static_cast<float>(i));
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

        shader.set("model", model);

        glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
    }

    // glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, nullptr);
}

// Quad with 2 triangles
uint32_t createVertexData(uint32_t *VBO, uint32_t *EBO)
{
    float vertices[] = {
        // Position                  // UVs
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Front
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Right
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Back
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // Left
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, // Bottom
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, // Top
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    uint32_t indices[] = {
        0, 1, 2, 0, 2, 3,       // Front
        4, 5, 6, 4, 6, 7,       // Right
        8, 9, 10, 8, 10, 11,    // Back
        12, 13, 14, 12, 14, 15, // Left
        16, 17, 18, 16, 18, 19, // Bottom
        20, 21, 22, 20, 22, 23  // Top
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Return the VAO
    return VAO;
}

uint32_t createTexture(const char *path)
{
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
        exit(EXIT_FAILURE);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}

int main()
{
    // Get window instance
    Window *window = Window::instance();

    // Create program
    const Shader shader(PROJECT_PATH "vertex.vert", PROJECT_PATH "fragment.frag");
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
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Display only lines
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    uint32_t tex = createTexture(ASSETS_PATH "textures/blue_blocks.jpg");
    uint32_t tex2 = createTexture(ASSETS_PATH "textures/bricks_arrow.jpg");

    // Main loop
    while (window->isAlive())
    {
        // handleInput();
        // render(program, VAO, numVertices);
        render(shader, VAO, 36, tex, tex2);
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
