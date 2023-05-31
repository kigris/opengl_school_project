#include "engine/light.hpp"
#include "engine/shader.hpp"
#include <string>

void Light::set(const Shader& shader, const char* prefixUniformName) const {
    std::string uniformName = std::string(prefixUniformName) + ".ambient";
    shader.set(uniformName.c_str(), _ambient);

    uniformName = std::string(prefixUniformName) + ".diffuse";
    shader.set(uniformName.c_str(), _diffuse);

    uniformName = std::string(prefixUniformName) + ".specular";
    shader.set(uniformName.c_str(), _specular);

    uniformName = std::string(prefixUniformName) + ".intensity";
    shader.set(uniformName.c_str(), _intensity);
}


void DirectionalLight::set(const Shader& shader, const char* prefixUniformName) const {
    Light::set(shader, prefixUniformName);
    std::string uniformName = std::string(prefixUniformName) + ".direction";
    shader.set(uniformName.c_str(), _direction);
}

void PointLight::set(const Shader& shader, const char* prefixUniformName) const {
    Light::set(shader, prefixUniformName);
    std::string uniformName = std::string(prefixUniformName) + ".position";
    shader.set(uniformName.c_str(), _position);

    uniformName = std::string(prefixUniformName) + ".constant";
    shader.set(uniformName.c_str(), _constant);

    uniformName = std::string(prefixUniformName) + ".linear";
    shader.set(uniformName.c_str(), _linear);

    uniformName = std::string(prefixUniformName) + ".quadratic";
    shader.set(uniformName.c_str(), _quadratic);
}
