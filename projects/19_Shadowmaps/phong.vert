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

// normal matrix for non-uniform scaling
uniform mat3 normalMat;

uniform mat4 lightSpaceMatrix;

out vec3 normal;
out vec3 fragPos;
out vec2 uv;
out vec4 fragPostLightSpace;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    normal = normalMat * aNormal;
    fragPos = vec3(model * vec4(aPos, 1.0f));
    fragPostLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0f);
    uv = aTexCoord;
}
