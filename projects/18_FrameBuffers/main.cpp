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
#include "engine/light.hpp"

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

DirectionalLight dirLight(
    glm::vec3(-0.2f, -1.0f, -0.3f),
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    0.8f);

PointLight pointLight(
    lightPos,
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.2f, 0.1f, 0.8f),
    glm::vec3(0.6f, 0.6f, 0.6f),
    0.4f,
    1.0f,
    0.9f,
    0.32f);

PointLight pointLight2(
    lightPos2,
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.1f, 0.8f, 0.3f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    1.0f,
    1.0f,
    0.9f,
    0.32f);

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

void render(const Shader &s_light, const Shader &s_phong, const Shader &s_phongCustomObj, const Geometry &cube, const Geometry &quad, const Model &obj, const Texture &tex_albedo, const Texture &tex_specular, const bool staticView = false)
{
    glm::mat4 view;
    if (!staticView)
        // Get the view matrix from camera
        view = camera.getViewMatrix();
    else {
        // Set fixed view matrix
        view = glm::lookAt(glm::vec3(0.0f, 2.0f, 4.0f), // Position
                           glm::vec3(0.0f, 0.0f, 0.0f), // Direction
                           glm::vec3(0.0f, 1.0f, 0.0f)  // World up
        );
    }

    glm::mat4 proj;
    // Set the projection matrix
    if(!staticView)
        // Get the projection matrix from camera
        proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
    else
        // Set fixed projection matrix (perspective)
        proj = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    // Light source
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

    // Floor
    s_phong.use();
    // Model matrix for the floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.35f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);
    // Set uniforms for the material
    tex_albedo.use(s_phong, "material.diffuse", 0);
    tex_specular.use(s_phong, "material.specular", 1);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);
    // Set the camera position
    if(!staticView)
        s_phong.set("viewPos", camera.getPosition());
    else
        s_phong.set("viewPos", glm::vec3(0.0f, 2.0f, 4.0f));
    quad.render();

    s_phong.set("material.shininess", 64);

    // Set uniforms for the material
    // Dir lights
    dirLight.set(s_phong, "dirLights[0]");
    // Point lights
    pointLight.set(s_phong, "pointLights[0]");
    pointLight2.set(s_phong, "pointLights[1]");

    for (auto &cubePos : cubePositions)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        s_phong.set("model", model);
        normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
        s_phong.set("normalMat", normalMat);
        cube.render();
    }

    s_phongCustomObj.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f * cos(glfwGetTime()), 0.0f, 1.0f * sin(glfwGetTime())));
    model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
    s_phongCustomObj.set("view", view);
    s_phongCustomObj.set("proj", proj);
    s_phongCustomObj.set("model", model);
    // Set dir lights
    dirLight.set(s_phongCustomObj, "dirLights[0]");
    // Point lights
    pointLight.set(s_phongCustomObj, "pointLights[0]");
    pointLight2.set(s_phongCustomObj, "pointLights[1]");
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phongCustomObj.set("normalMat", normalMat);
    s_phongCustomObj.set("viewPos", camera.getPosition());
    s_phongCustomObj.set("material.shininess", 64);
    obj.render(s_phongCustomObj);
}

int main()
{
    // Get window instance
    Window *window = Window::instance();
    // Create cube
    const Cube cube(0.75f);
    const Quad quad(10.0f);
    const Quad renderQuad(0.5f, 0.75f, -0.75f, 1.0f);
    // Create shaders
    const Shader s_light(PROJECT_PATH "light.vert", PROJECT_PATH "light.frag");
    const Shader s_phong(PROJECT_PATH "phong.vert", PROJECT_PATH "blinn.frag");
    const Shader s_phongCustomObj(PROJECT_PATH "phongCustomObj.vert", PROJECT_PATH "blinnCustomObj.frag");
    const Shader s_projection(PROJECT_PATH "projection.vert", PROJECT_PATH "projection.frag");
    // Import model
    const Model object(ASSETS_PATH "models/Freighter/Freigther_BI_Export.obj");
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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

    // Framebuffer
    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Link textures to framebuffer
    // Color buffer
    uint32_t texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    // Set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->getWidth(), window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
    // Depth buffer
    uint32_t rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    // Set renderbuffer data
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->getWidth(), window->getHeight());
    // Unbind renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // Attach renderbuffer to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR:: Framebuffer is not complete!" << std::endl;

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Main loop
    while (window->isAlive())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        handleInput(deltaTime);

        // First pass, render the scene to the default framebuffer, free camera
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Render the entire scene to the screen (using default framebuffer)
        render(s_light, s_phong, s_phongCustomObj, cube, quad, object, tex_albedo, tex_specular);

        // Second pass, render the scene to a custom framebuffer, fixed camera
        // Switch to your custom framebuffer and render the scene again
        glEnable(GL_DEPTH_TEST); // Depth testing must be enabled
        // Bind the custom framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Resize the framebuffer COLOR texture to match the window size
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->getWidth(), window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        // Resize the renderbuffer DEPTH texture to match the window size
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->getWidth(), window->getHeight());

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Render the entire scene to the custom framebuffer
        render(s_light, s_phong, s_phongCustomObj, cube, quad, object, tex_albedo, tex_specular, true);

        // Switch back to the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Disable depth test
        glDisable(GL_DEPTH_TEST);
        // Render the smaller overlay (using the texture from the custom framebuffer)
        s_projection.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        s_projection.set("screenTexture", 0);
        // Render the quad with the texture
        renderQuad.render();

        window->frame();
    }

    // Delete framebuffer
    glDeleteFramebuffers(1, &fbo);

    return 0;
}
