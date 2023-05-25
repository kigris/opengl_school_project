#version 330 core

// Output color
out vec4 FragColor;
uniform vec4 color;

void main()
{
    FragColor = color;
}