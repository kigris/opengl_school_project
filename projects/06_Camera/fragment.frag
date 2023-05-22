#version 330 core

// Input texture coordinates
in vec2 texCoord;
// Output color
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);
}