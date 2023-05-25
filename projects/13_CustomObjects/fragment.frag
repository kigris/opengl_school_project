#version 330 core

// Input texture coordinates
in vec2 uv;
// Output color
out vec4 FragColor;

uniform sampler2D texture_diffuse;

void main()
{
    // FragColor = mix(texture(texture1, uv), texture(texture2, uv), 0.5);
    FragColor = texture(texture_diffuse, uv);
}