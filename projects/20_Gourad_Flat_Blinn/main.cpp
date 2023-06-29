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
#include <GLFW/glfw3.h>0
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
#include "engine/geometry/sphere.hpp"
#include "engine/camera.hpp"
#include "engine/texture.hpp"

bool firstMouse = true;
double lastX, lastY;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightPos2(-1.2f, 1.0f, 2.0f);

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

void onScrollCallback(double xoffset, double yoffset)
{
    camera.handleMouseScroll(yoffset);
}

void handleInput(float deltaTime)
{
    Input *input = Input::instance();
    if (input->isKeyPressed(GLFW_KEY_W))
    {
        camera.handleKeyboard(Camera::Movement::Forward, deltaTime);
    }
    if (input->isKeyPressed(GLFW_KEY_S))
    {
        camera.handleKeyboard(Camera::Movement::Backward, deltaTime);
    }
    if (input->isKeyPressed(GLFW_KEY_A))
    {
        camera.handleKeyboard(Camera::Movement::Left, deltaTime);
    }
    if (input->isKeyPressed(GLFW_KEY_D))
    {
        camera.handleKeyboard(Camera::Movement::Right, deltaTime);
    }
}

// Render VAO
void render(const Shader &s_light, const Shader &s_phong, const Shader &s_gourad, const Shader &s_flat, const Geometry &sphere, const Texture &tex_albedo, const Texture &tex_specular)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Get the view matrix from camera
    glm::mat4 view = camera.getViewMatrix();
    // Set the projection matrix
    const glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
    s_light.use();
    // Circular movement
    float l_pos[] = {std::sin((float)glfwGetTime()), 0.0f, std::cos((float)glfwGetTime())};
    // Far factor
    float factor = 3;
    lightPos = glm::vec3(l_pos[0]*factor, l_pos[1]*factor, l_pos[2]*factor);
    // Model matrix for the light source
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    s_light.set("model", model);
    s_light.set("view", view);
    s_light.set("proj", proj);
    sphere.render();

    // Flat model
    s_flat.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.75f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_flat.set("model", model);
    s_flat.set("view", view);
    s_flat.set("proj", proj);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_flat.set("normalMat", normalMat);
    s_flat.set("viewPos", camera.getPosition());
    // Set uniforms for the material
    tex_albedo.use(s_flat, "material.diffuse", 0);
    tex_specular.use(s_flat, "material.specular", 1);
    // s_phong.set("material.diffuse", 0);
    // s_phong.set("material.specular", 1);
    s_flat.set("material.shininess", 128);

    // Set uniforms for the material
    s_flat.set("light.position", lightPos);
    s_flat.set("light.ambient", 0.1f, 0.1f, 0.1f);
    s_flat.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_flat.set("light.specular", 1.0f, 1.0f, 1.0f);
    s_flat.set("light.constant", 1.0f);
    s_flat.set("light.linear", 0.09f);
    s_flat.set("light.quadratic", 0.032f);
    sphere.render();

    // // Gourad model
    s_gourad.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_gourad.set("model", model);
    s_gourad.set("view", view);
    s_gourad.set("proj", proj);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_gourad.set("normalMat", normalMat);
    s_gourad.set("viewPos", camera.getPosition());
    // Set uniforms for the material
    tex_albedo.use(s_gourad, "material.diffuse", 0);
    tex_specular.use(s_gourad, "material.specular", 1);
    // s_phong.set("material.diffuse", 0);
    // s_phong.set("material.specular", 1);
    s_gourad.set("material.shininess", 128);

    // Set uniforms for the material
    s_gourad.set("light.position", lightPos);
    s_gourad.set("light.ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_gourad.set("light.specular", 1.0f, 1.0f, 1.0f);
    s_gourad.set("light.constant", 1.0f);
    s_gourad.set("light.linear", 0.09f);
    s_gourad.set("light.quadratic", 0.032f);
    sphere.render();

    // Phong-Blinn model
    s_phong.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.75f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);
    s_phong.set("viewPos", camera.getPosition());
    // Set uniforms for the material
    tex_albedo.use(s_phong, "material.diffuse", 0);
    tex_specular.use(s_phong, "material.specular", 1);
    // s_phong.set("material.diffuse", 0);
    // s_phong.set("material.specular", 1);
    s_phong.set("material.shininess", 128);

    // Set uniforms for the material
    s_phong.set("light.position", lightPos);
    s_phong.set("light.ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);
    s_phong.set("light.constant", 1.0f);
    s_phong.set("light.linear", 0.09f);
    s_phong.set("light.quadratic", 0.032f);
    sphere.render();
}

int main()
{
    // Get window instance
    Window *window = Window::instance();
    // Create cube
    const Sphere sphere(0.75f, 50, 50);
    // Create program
    const Shader s_light(PROJECT_PATH "light.vert", PROJECT_PATH "light.frag");
    const Shader s_phong(PROJECT_PATH "phong.vert", PROJECT_PATH "blinn.frag");
    const Shader s_gourad(PROJECT_PATH "gourad.vert", PROJECT_PATH "gourad.frag");
    const Shader s_flat(PROJECT_PATH "flat.vert", PROJECT_PATH "flat.frag");
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
        render(s_light, s_phong, s_gourad, s_flat, sphere, tex_albedo, tex_specular);
        window->frame();
    }

    return 0;
}
