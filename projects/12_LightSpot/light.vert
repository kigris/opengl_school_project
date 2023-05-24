#version 330 core
// position attribute
layout(location = 0) in vec3 aPos;
// texture coord attribute
layout(location = 1) in vec2 aTexCoord;
// normal coord attribute
layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}