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
#include "engine/geometry/cube.hpp"
#include "engine/geometry/quad.hpp"
#include "engine/camera.hpp"
#include "engine/texture.hpp"

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
glm::vec3 lightPos(5.2f, 3.5f, 0.0f);

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

void render(const Shader &s_phong, const Shader &s_light, const Geometry &cube, const Geometry &quad, const Texture& tex_albedo, const Texture& tex_specular, uint32_t depthTex)
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

    const float k_shadow_near = 0.5f;
    const float k_shadow_far = 7.5f;
    // Set the orthographic projection matrix
    const glm::mat4 projLight = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, k_shadow_near, k_shadow_far);
    // Get the view matrix from light
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = projLight * lightView;

    // Floor render
    s_phong.use();
    s_phong.set("lightSpaceMatrix", lightSpaceMatrix);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);
    s_phong.set("viewPos", camera.getPosition());
    // Set uniforms for the materialgit 
    tex_albedo.use(s_phong, "material.diffuse", 0);
    tex_specular.use(s_phong, "material.specular", 1);
    s_phong.set("material.diffuse", 0);
    s_phong.set("material.specular", 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    s_phong.set("depthMap", 2);
    s_phong.set("material.shininess", 128);

    // Set uniforms for the material
    s_phong.set("light.position", lightPos);
    s_phong.set("light.ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);
    // s_phong.set("light.constant", 1.0f);
    // s_phong.set("light.linear", 0.09f);
    // s_phong.set("light.quadratic", 0.032f);
    quad.render();

    for(auto& cubePos : cubePositions) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        s_phong.set("model", model);
        normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
        s_phong.set("normalMat", normalMat);
        cube.render();
    }
}

void lightRender(const Shader &s_depth, const Geometry &cube, const Geometry &quad)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float k_shadow_near = 0.5f;
    const float k_shadow_far = 7.5f;
    // Set the orthographic projection matrix
    const glm::mat4 projLight = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, k_shadow_near, k_shadow_far);

    // Get the view matrix from light
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = projLight * lightView;

    s_depth.use();
    s_depth.set("lightSpaceMatrix", lightSpaceMatrix);
     // Floor render
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_depth.set("model", model);
    quad.render();

    for(auto& cubePos : cubePositions) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        s_depth.set("model", model);
        cube.render();
    }
}

int main()
{
    // Get window instance
    Window *window = Window::instance();
    // Create cube
    const Cube cube(0.75f);
    const Quad quad(10.0f);
    // Create program
    const Shader s_phong(PROJECT_PATH "phong.vert", PROJECT_PATH "blinn.frag");
    const Shader s_light(PROJECT_PATH "light.vert", PROJECT_PATH "light.frag");
    const Shader s_shadow(PROJECT_PATH "shadow.vert", PROJECT_PATH "shadow.frag");
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create framebuffer
    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    // Create texture
    uint32_t depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
    // Set draw buffers
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete!" << std::endl;
    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

        // 1st pass
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, 1024, 1024);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        lightRender(s_shadow, cube, quad);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2nd pass
        glViewport(0, 0, window->getWidth(), window->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render(s_phong, s_light, cube, quad, tex_albedo, tex_specular, depthTex);
        window->frame();
    }

    return 0;
}
