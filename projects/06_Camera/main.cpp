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

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = (float)800 / 2.0;
float lastY = (float)600 / 2.0;
float fov = 90.0f;

// Position of the camera in world space
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// This is where the camera looks at, it is a vector that points to the direction
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// Vector up of world space
const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// Vector right of world space, cross product of cameraFront and cameraUp
const glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
const float camSpeed = 2.0f;

void onMouseCallback(GLFWwindow *window, double xpos, double ypos)
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

    float sensitivity = 0.05f; // constraint movement speed
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset; // add offsets to pitch/yaw
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front; // calculate the direction vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  if (fov >= 1.0f && fov <= 90.0f)
    fov -= yoffset;
  if (fov <= 1.0f)
    fov = 1.0f;
  if (fov >= 90.0f)
    fov = 90.0f;
}

void handleInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float speed = camSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= speed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += speed * cameraRight;
}

glm::mat4 lookAt(const glm::vec3 &cameraPosition, const glm::vec3 &targetPosition, const glm::vec3 &upDirection)
{
    // Calculate the forward vector (from the camera to the target)
    glm::vec3 forwardVector(glm::normalize(targetPosition - cameraPosition));
    // Calculate the right vector (perpendicular to the forward vector and up direction)
    glm::vec3 rightVector(glm::normalize(glm::cross(forwardVector, upDirection)));
    // Recalculate the orthonormal up vector (perpendicular to the right and forward vectors)
    glm::vec3 orthonormalUpVector(glm::cross(rightVector, forwardVector));
    // Create the lookAt matrix
    glm::mat4 lookAtMatrix(1);  // Start with an identity matrix
    // Set the right, up, and forward vectors in the matrix
    // Note: These vectors become the columns of the rotation matrix portion of the lookAt matrix
    lookAtMatrix[0] = glm::vec4(rightVector, 0);
    lookAtMatrix[1] = glm::vec4(orthonormalUpVector, 0);
    lookAtMatrix[2] = glm::vec4(-forwardVector, 0);
    lookAtMatrix = glm::transpose(lookAtMatrix);  // Transpose the matrix to get the correct rotation

    // Set the camera position in the matrix
    glm::mat4 translation(1);
    translation[3][0] = -cameraPosition.x;
    translation[3][1] = -cameraPosition.y;
    translation[3][2] = -cameraPosition.z;
    // Combine the lookAt and translation matrices to create the view matrix
    lookAtMatrix = lookAtMatrix * translation;
    return lookAtMatrix;
}

// Render VAO
void render(const Shader &shader, const Sphere &sphere, int n, uint32_t tex, uint32_t tex2)
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
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.5f, 0.5f));

    // float radius = 3.0f;
    // float camX = sin(glfwGetTime()) * radius;
    // float camZ = cos(glfwGetTime()) * radius;
    // glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camZ),
    //                             glm::vec3(0.0f, 0.0f, 0.0f),
    //                             glm::vec3(0.0f, 1.0f, 0.0f));

    // const glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),  // position of camera
    //                          glm::vec3(0.0f, 0.0f, 0.0f),  // point to look
    //                          glm::vec3(0.0f, 1.0f, 0.0f)); // up vector

    // glm::mat4 view = glm::mat4(1.0f);
    // view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Create view matrix using lookAt function
    glm::mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    const glm::mat4 proj = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
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
    // Mouse callback
    glfwSetCursorPosCallback(window->getNativeWindow(), onMouseCallback);
    glfwSetScrollCallback(window->getNativeWindow(), onScrollCallback);
    glfwSetInputMode(window->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    // Main loop
    while (window->isAlive())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        handleInput(window->getNativeWindow(), deltaTime);
        // render(program, VAO, numVertices);
        render(shader, sphere, 36, tex, tex2);
        // render(program, VAO[0], 3);
        // render(program2, VAO[1], 3);
        window->frame();
    }

    return 0;
}
