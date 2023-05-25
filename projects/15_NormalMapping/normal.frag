#version 330 core

// Output of the fragment color
out vec4 FragColor;

// Input from the vertex shader
in vec2 uv;

in vec3 tangentFragPos;
in vec3 tangentLightPos;
in vec3 tangentViewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    int shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // Sample normal
    vec3 normal = vec3(texture(material.normal, uv));
    normal = normalize(normal * 2 - 1.0f);

    // Albeto color
    vec3 albedo = vec3(texture(material.diffuse, uv));
    // Ambient color
    vec3 ambient = light.ambient * albedo;
    // Convert light position to view space
    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo;
    // Specular color
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));

    vec3 phong = ambient + diffuse + specular;
    FragColor = vec4(phong, 1.0);
}