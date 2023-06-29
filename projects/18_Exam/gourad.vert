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

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    int shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
out vec2 uv;
out vec3 diffuseOut;
out vec3 specularOut;
out float attenuationOut;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    vec3 normal = normalMat * aNormal;
    // vec3 fragPos = vec3(model * vec4(aPos, 1.0f));
    uv = aTexCoord;
    
    // Diffuse color
    vec3 norm = normalize(normal);
    vec3 vertPos = vec3(model * vec4(aPos, 1.0f));
    vec3 lightDir = normalize(light.position - vertPos);
    float diff = max(dot(norm, lightDir), 0.0);
    diffuseOut = light.diffuse * diff;
    // Specular color
    vec3 viewDir = normalize(viewPos - vertPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    specularOut = light.specular * spec;

    float distance = length(light.position - vertPos);
    attenuationOut = 1.0 / (light.constant + light.linear * distance +
                    light.quadratic * (distance * distance));

    // phong = (ambient + diffuse + specular) * attenuation;
}
