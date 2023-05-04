#version 330 core
// position attribute
layout(location = 0) in vec3 aPos;
// color attribute
layout(location = 1) in vec3 aColor;
// texture coord attribute
layout(location = 2) in vec2 aTexCoord;

// output to fragment shader
out vec3 myColor;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    myColor = aColor;
    texCoord = vec2(aTexCoord.x*2, -aTexCoord.y*2);
}