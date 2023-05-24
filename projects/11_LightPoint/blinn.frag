#version 330 core

// Output of the fragment color
out vec4 FragColor;

// Input from the vertex shader
in vec3 normal;
in vec3 fragPos;
in vec2 uv;

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

void main() {
    // Albeto color
    vec3 albedo = vec3(texture(material.diffuse, uv));
    // Ambient color
    vec3 ambient = light.ambient * albedo;
    // Diffuse color
    vec3 norm = normalize(normal);
    // Convert light position to view space
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo;
    // Specular color
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                    light.quadratic * (distance * distance));

    vec3 phong = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(phong, 1.0);
}