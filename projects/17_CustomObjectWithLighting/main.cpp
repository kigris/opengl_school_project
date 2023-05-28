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
#include "engine/geometry/cube.hpp"
#include "engine/geometry/quad.hpp"
#include "engine/camera.hpp"
#include "engine/texture.hpp"
#include "engine/model.hpp"

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, -4.0f),
    glm::vec3(0.0f, 0.0f, 4.0f),
    glm::vec3(-4.0f, 0.0f, 0.0f),
    glm::vec3(4.0f, 0.0f, 0.0f),
    glm::vec3(4.0f, 0.0f, 4.0f),
    glm::vec3(4.0f, 0.0f, -4.0f),
    glm::vec3(-4.0f, 0.0f, 4.0f),
    glm::vec3(-4.0f, 0.0f, -4.0f),
};

bool firstMouse = true;
double lastX, lastY;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(0.0f, 1.0f, 3.0f);
glm::vec3 lightPos2(0.0f, 1.0f, -3.0f);

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
void render(const Shader &s_light, const Shader &s_phong, const Shader &s_phongCustomObj, const Geometry &cube, const Geometry &quad, const Model& obj, const Texture &tex_albedo, const Texture &tex_specular)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Get the view matrix from camera
    glm::mat4 view = camera.getViewMatrix();
    // Set the projection matrix
    const glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
    s_light.use();
    // Model matrix for the light source
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    s_light.set("model", model);
    s_light.set("view", view);
    s_light.set("proj", proj);
    cube.render();
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos2);
    model = glm::scale(model, glm::vec3(0.2f));
    s_light.set("model", model);
    cube.render();

    s_phong.use();
    // Model matrix for the light source
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);
    // Set uniforms for the material
    tex_albedo.use(s_phong, "material.diffuse", 0);
    tex_specular.use(s_phong, "material.specular", 1);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);
    s_phong.set("viewPos", camera.getPosition());
    quad.render();
    

    // s_phong.set("material.diffuse", 0);
    // s_phong.set("material.specular", 1);
    s_phong.set("material.shininess", 128);

    // Set uniforms for the material
    // Dir lights
    s_phong.set("dirLights[0].direction", 0.3f, -1.0f, 0.0f);
    s_phong.set("dirLights[0].ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("dirLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    s_phong.set("dirLights[0].specular", 0.4f, 0.4f, 0.4f);
    s_phong.set("dirLights[0].intensity", 0.2f);
    // Point lights
    s_phong.set("pointLights[0].position", lightPos);
    s_phong.set("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    s_phong.set("pointLights[0].constant", 1.0f);
    s_phong.set("pointLights[0].linear", 0.9f);
    s_phong.set("pointLights[0].quadratic", 0.32f);
    s_phong.set("pointLights[1].position", lightPos2);
    s_phong.set("pointLights[1].ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("pointLights[1].diffuse", 0.0f, 0.5f, 0.5f);
    s_phong.set("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    s_phong.set("pointLights[1].constant", 1.0f);
    s_phong.set("pointLights[1].linear", 0.9f);
    s_phong.set("pointLights[1].quadratic", 0.32f);


    for (auto &cubePos : cubePositions)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        s_phong.set("model", model);
        normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
        s_phong.set("normalMat", normalMat);
        cube.render();
    }
    // Custom object part
    s_phongCustomObj.use();
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    s_phongCustomObj.set("view", view);
    s_phongCustomObj.set("proj", proj);
    s_phongCustomObj.set("model", model);
    s_phongCustomObj.set("dirLights[0].direction", 0.3f, -0.7f, 0.4f);
    s_phongCustomObj.set("dirLights[0].ambient", 0.1f, 0.1f, 0.1f);
    s_phongCustomObj.set("dirLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    s_phongCustomObj.set("dirLights[0].specular", 0.4f, 0.4f, 0.4f);
    s_phongCustomObj.set("dirLights[0].intensity", 0.4f);
    // Point lights
    s_phongCustomObj.set("pointLights[0].position", lightPos);
    s_phongCustomObj.set("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
    s_phongCustomObj.set("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
    s_phongCustomObj.set("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    s_phongCustomObj.set("pointLights[0].constant", 1.0f);
    s_phongCustomObj.set("pointLights[0].linear", 0.09f);
    s_phongCustomObj.set("pointLights[0].quadratic", 0.032f);
    s_phongCustomObj.set("pointLights[1].position", lightPos2);
    s_phongCustomObj.set("pointLights[1].ambient", 0.1f, 0.1f, 0.1f);
    s_phongCustomObj.set("pointLights[1].diffuse", 0.0f, 0.5f, 0.5f);
    s_phongCustomObj.set("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    s_phongCustomObj.set("pointLights[1].constant", 1.0f);
    s_phongCustomObj.set("pointLights[1].linear", 0.09f);
    s_phongCustomObj.set("pointLights[1].quadratic", 0.032f);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phongCustomObj.set("normalMat", normalMat);
    s_phongCustomObj.set("viewPos", camera.getPosition());
    s_phongCustomObj.set("material.shininess", 256);
    obj.render(s_phongCustomObj);
}

int main()
{
    // Get window instance
    Window *window = Window::instance();
    // Create cube
    const Cube cube(0.75f);
    const Quad quad(10.0f);
    // Create shaders
    const Shader s_light(PROJECT_PATH "light.vert", PROJECT_PATH "light.frag");
    const Shader s_phong(PROJECT_PATH "phong.vert", PROJECT_PATH "blinn.frag");
    const Shader s_phongCustomObj(PROJECT_PATH "phongCustomObj.vert", PROJECT_PATH "blinnCustomObj.frag");
    // Import model
    const Model object(ASSETS_PATH "models/Freighter/Freigther_BI_Export.obj");
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
        render(s_light, s_phong, s_phongCustomObj, cube, quad, object, tex_albedo, tex_specular);
        window->frame();
    }

    return 0;
}
