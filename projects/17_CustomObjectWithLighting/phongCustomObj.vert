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

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
}; 

struct DirLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float intensity;
};

// Define with the number of lights
#define NUMBER_DIR_LIGHTS 1
#define NUMBER_POINT_LIGHTS 2
uniform DirLight dirLights[NUMBER_DIR_LIGHTS];
uniform PointLight pointLights[NUMBER_POINT_LIGHTS];
uniform vec3 viewPos;

out vec2 uv;
out vec3 tangentFragPos;
out vec3 tangentDirLightPos[NUMBER_DIR_LIGHTS];
out vec3 tangentPointLightPos[NUMBER_POINT_LIGHTS];
out vec3 tangentViewPos;

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
    tangentViewPos = TBN * viewPos;

    for(int i = 0; i < NUMBER_DIR_LIGHTS; i++)
        tangentDirLightPos[i] = TBN * dirLights[i].direction;

    for(int i = 0; i < NUMBER_POINT_LIGHTS; i++)
        tangentPointLightPos[i] = TBN * pointLights[i].position;

    gl_Position = proj * view * model * vec4(aPos, 1.0f);
}
