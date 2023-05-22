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
#define _USE_MATH_DEFINES
#include <math.h>
#include "engine/window.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"

void handleInput()
{
}

// Render VAO
void render(const Shader &shader, uint32_t VAO, int n)
{
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, nullptr);
}

// Quad with 2 triangles
uint32_t createVertexData(uint32_t *VBO, uint32_t *EBO)
{
    float vertices[] = {
        // positions            // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
    };

    uint32_t indices[] = {
        0, 2, 1, // first triangle
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Return the VAO
    return VAO;
}

uint32_t createProceduralNSideObject(uint32_t *VBO, uint32_t *EBO, uint32_t *numVertices, int sides, float radius, float offX, float offY)
{
    // Create vertex data
    // TODO: Dyanmic illegal allocation
    // float* vertices = (float*)malloc(sizeof(float) * sides * 3);^
    float *__restrict vertices = new float[sides * 3];
    int verticesSize = sizeof(float) * sides * 3;
    uint32_t *indices = new uint32_t[(sides - 2) * 3];
    int indicesSize = sizeof(uint32_t) * (sides - 2) * 3;
    *numVertices = uint32_t((sides - 2) * 3);

    // Calculate angle between vertices in degrees
    float angle = 360.0f / (float)sides;
    // Convert angle to radians
    float angleRad = angle * M_PI / 180.0f;

    // Create vertices
    for (int i = 0; i < sides; i++)
    {
        vertices[i * 3] = radius * cos(angleRad * (float)i) + offX;
        vertices[i * 3 + 1] = radius * sin(angleRad * (float)i) + offY;
        vertices[i * 3 + 2] = 0.0f;
    }

    // Create indices
    uint32_t curr_exp = 0;
    uint32_t counter = 0;
    uint32_t curr_pow = 0;
    uint32_t shapeSides = uint32_t(sides);
    while (shapeSides >= 3)
    {
        curr_pow = pow(2, curr_exp);
        for (int i = 0; (curr_pow * (i + 1)) < sides; i += 2)
        {
            indices[counter] = curr_pow * i;
            indices[counter + 1] = (curr_pow * (i + 1));
            if ((curr_pow * (i + 2)) >= sides)
            {
                indices[counter + 2] = 0;
                counter += 3;
                break;
            }
            else
                indices[counter + 2] = (curr_pow * (i + 2));
            counter += 3;
        }
        shapeSides = (uint32_t)(ceil((float)shapeSides / 2.0f));
        curr_exp++;
    }

    uint32_t VAO;
    glGenBuffers(1, VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    delete [] vertices;
    delete [] indices;
    return VAO;
}

int main()
{
    // Get window instance
    Window *window = Window::instance();

    // Create program
    const Shader shader("../../projects/03_Shaders/vertex.vert", "../../projects/03_Shaders/fragment.frag");
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

    // Main loop
    while (window->isAlive())
    {
        handleInput();
        // render(program, VAO, numVertices);
        render(shader, VAO, 3);
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
