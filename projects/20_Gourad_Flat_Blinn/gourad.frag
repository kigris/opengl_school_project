#version 330 core

// Output of the fragment color
out vec4 FragColor;

// Input from the vertex shader
// in vec3 fragPos;
in vec2 uv;
in vec3 diffuseOut;
in vec3 specularOut;
in float attenuationOut;

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

uniform Light light;
uniform Material material;

void main() {
    // Albedo color
    vec3 albedoTex = vec3(texture(material.diffuse, uv));
    // Ambient color
    vec3 ambient = light.ambient * albedoTex;
    // Spec color
    vec3 specularTex = vec3(texture(material.specular, uv));

    vec3 diffuse = diffuseOut * albedoTex;
    vec3 specular = specularOut * specularTex;

    vec3 phong = (ambient + diffuse + specular) * attenuationOut;
    FragColor = vec4(phong, 1.0);
}