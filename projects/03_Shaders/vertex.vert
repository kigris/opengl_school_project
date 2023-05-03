#version 330 core
// position attribute
layout(location = 0) in vec3 aPos;
// color attribute
layout(location = 1) in vec3 aColor;

// output to fragment shader
out vec3 myColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    myColor = aColor;
}