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
#include "engine/input.hpp"
#include "engine/window.hpp"
#include "engine/shader.hpp"
#include "engine/geometry/quad.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/camera.hpp"
#include "engine/texture.hpp"

bool firstMouse = true;
double lastX, lastY;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void onMouseCallback(double xpos, double ypos)
{
    if (firstMouse)
    { // initial values the first frame
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX; // calc offset movement since last frame
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollCallback(double xoffset, double yoffset) {
  camera.handleMouseScroll(yoffset);
}

void handleInput(float deltaTime)
{
    Input* input = Input::instance();
    if(input->isKeyPressed(GLFW_KEY_W)) {
        camera.handleKeyboard(Camera::Movement::Forward, deltaTime);
    }
    if(input->isKeyPressed(GLFW_KEY_S)) {
        camera.handleKeyboard(Camera::Movement::Backward, deltaTime);
    }
    if(input->isKeyPressed(GLFW_KEY_A)) {
        camera.handleKeyboard(Camera::Movement::Left, deltaTime);
    }
    if(input->isKeyPressed(GLFW_KEY_D)) {
        camera.handleKeyboard(Camera::Movement::Right, deltaTime);
    }
}

// Render VAO
void render(const Shader &s_light, const Shader &s_phong, const Shader &s_normal, const Geometry& quad, const Geometry& cube, const Texture& tex_albedo, const Texture& tex_specular, const Texture& tex_normal)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Get the view matrix from camera
    glm::mat4 view = camera.getViewMatrix();
    // Set the projection matrix
    const glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
    s_light.use();

    float l_pos[] = {std::sin((float)glfwGetTime()/2.0f), 0.0f, std::abs(std::cos((float)glfwGetTime()/2.0f))};
    lightPos = glm::vec3(l_pos[0], l_pos[1], l_pos[2]);

    // Model matrix for the light source
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    s_light.set("model", model);
    s_light.set("view", view);
    s_light.set("proj", proj);
    cube.render();

    // Blinn model without normal mapping
    s_phong.use();
    // Model matrix for the light source
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);
    s_phong.set("viewPos", camera.getPosition());
    // Set uniforms for the material
    tex_albedo.use(s_phong, "material.diffuse", 0);
    tex_specular.use(s_phong, "material.specular", 1);
    s_phong.set("material.diffuse", 0);
    s_phong.set("material.specular", 1);
    s_phong.set("material.shininess", 128);

    // Set uniforms for the material
    s_phong.set("light.position", lightPos);
    s_phong.set("light.ambient", 0.2f, 0.2f, 0.2f);
    s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);
    quad.render();

    // Blinn model WITH normal mapping
    s_normal.use();
    // Model matrix for the light source
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
    s_normal.set("model", model);
    s_normal.set("view", view);
    s_normal.set("proj", proj);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_normal.set("normalMat", normalMat);
    s_normal.set("viewPos", camera.getPosition());
    // Set uniforms for the material
    tex_albedo.use(s_normal, "material.diffuse", 0);
    tex_specular.use(s_normal, "material.specular", 1);
    s_normal.set("material.diffuse", 0);
    s_normal.set("material.specular", 1);
    s_normal.set("material.normal", 2);
    s_normal.set("material.shininess", 128);

    tex_normal.use(s_normal, "material.normal", 2);

    // Set uniforms for the material
    s_normal.set("light.position", lightPos);
    s_normal.set("light.ambient", 0.2f, 0.2f, 0.2f);
    s_normal.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_normal.set("light.specular", 1.0f, 1.0f, 1.0f);
    quad.render();
}

int main()
{
    // Get window instance
    Window *window = Window::instance();
    // Create sphere
    const Quad quad(2.0f);
    const Cube cube(0.75f);
    // Create program
    const Shader s_light(PROJECT_PATH "light.vert", PROJECT_PATH "light.frag");
    const Shader s_phong(PROJECT_PATH "phong.vert", PROJECT_PATH "blinn.frag");
    const Shader s_normal(PROJECT_PATH "normal.vert", PROJECT_PATH "normal.frag");
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Display only lines
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    const Texture tex_albedo(ASSETS_PATH "textures/bricks_albedo.png", Texture::Format::RGB);
    const Texture tex_specular(ASSETS_PATH "textures/bricks_specular.png", Texture::Format::RGB);
    const Texture tex_normal(ASSETS_PATH "textures/bricks_normal.png", Texture::Format::RGB);
    // Set mouse callback
    Input::instance()->setMouseMoveCallback(onMouseCallback);
    Input::instance()->setMouseScrollCallback(onScrollCallback);

    glfwSetInputMode(window->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    // Main loop
    while (window->isAlive())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        handleInput(deltaTime);
        render(s_light, s_phong, s_normal, quad, cube, tex_albedo, tex_specular, tex_normal);
        window->frame();
    }

    return 0;
}
