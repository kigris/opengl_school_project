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

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLights[NUMBER_DIR_LIGHTS];
uniform PointLight pointLights[NUMBER_POINT_LIGHTS];

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // Albedo color
    vec3 albedo = vec3(texture(material.diffuse, uv));
    // Ambient color
    vec3 ambient = light.ambient * albedo;
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo;
    // Specular color
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                    light.quadratic * (distance * distance));
    vec3 phong = (ambient + diffuse + specular) * attenuation;
    return phong;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 albedo = vec3(texture(material.diffuse, uv));
    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));
    vec3 phong = (ambient + diffuse + specular) * light.intensity;
    return phong;
}

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 color = vec3(0.0);
    for(int i=0; i < NUMBER_DIR_LIGHTS; i++)
        color += calcDirLight(dirLights[i], norm, viewDir);
    for(int i=0; i < NUMBER_POINT_LIGHTS; i++)
        color += calcPointLight(pointLights[i], norm, fragPos, viewDir);

    FragColor = vec4(color, 1.0);
}