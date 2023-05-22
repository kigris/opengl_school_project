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
#include "engine/geometry/sphere.hpp"

// Render VAO
void render(const Shader &shader, const Sphere& sphere, int n, uint32_t tex, uint32_t tex2)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);

    shader.set("texture1", 0);
    shader.set("texture2", 1);

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
    sphere.render();
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

    // Create sphere
    const Sphere sphere(0.75f, 50, 50);
    // Create program
    const Shader shader(PROJECT_PATH "vertex.vert", PROJECT_PATH "fragment.frag");
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
        render(shader, sphere, 36, tex, tex2);
        // render(program, VAO[0], 3);
        // render(program2, VAO[1], 3);
        window->frame();
    }

    return 0;
}
