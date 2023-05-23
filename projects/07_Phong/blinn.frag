#version 330 core

// Output of the fragment color
out vec4 FragColor;

// Input from the vertex shader
in vec3 normal;
in vec3 fragPos;

// Uniforms
uniform mat4 viewMatrix;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
// Material properties
uniform float ambientStrength;
uniform int shininess;
uniform float specularStrength;

void main() {
    // Ambient color
    vec3 ambient = ambientStrength * lightColor;
    // Diffuse color
    vec3 norm = normalize(normal);
    // Convert light position to view space
    vec3 lightPosView = vec3(viewMatrix * vec4(lightPos, 1.0));
    vec3 lightDir = normalize(lightPosView - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular color
    // Convert camera position to view space
    vec3 viewPosView = vec3(viewMatrix * vec4(viewPos, 1.0));
    vec3 viewDir = normalize(viewPosView - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 phong = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(phong, 1.0);
}