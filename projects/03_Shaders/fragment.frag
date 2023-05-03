#version 330 core

// Input color
in vec3 myColor;
// Output color
out vec4 FragColor;

void main()
{
    FragColor = vec4(myColor, 1.0f);
}