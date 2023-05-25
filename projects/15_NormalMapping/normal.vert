#version 330 core

// position attribute
layout(location = 0) in vec3 aPos;
// texture coord attribute
layout(location = 1) in vec2 aTexCoord;
// normal coord attribute
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBiTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// normal matrix for non-uniform scaling
uniform mat3 normalMat;

out vec3 tangentFragPos;
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec2 uv;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;

void main()
{
    uv = aTexCoord;
    vec3 fragPos = vec3(model * vec4(aPos, 1.0f));

    vec3 T = normalize(normalMat * aTangent);
    vec3 N = normalize(normalMat * aNormal);
    T = normalize(T-dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    tangentFragPos = TBN * fragPos;
    tangentLightPos = TBN * light.position;
    tangentViewPos = TBN * viewPos;

    gl_Position = proj * view * model * vec4(aPos, 1.0f);
}
