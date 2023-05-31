#ifndef light_hpp
#define light_hpp

#include <glm/glm.hpp>

class Shader;

class Light {
public:
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity) :
        _ambient(ambient), _diffuse(diffuse), _specular(specular), _intensity(intensity) {}


    // Function to render
    virtual void set(const Shader& shader, const char* prefixUniformName) const;
private:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    float _intensity;
};

class DirectionalLight : public Light {
public:
    DirectionalLight() = delete;
    // Use this constructor to create a directional light with the base constructor of Light
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity) : Light(ambient, diffuse, specular, intensity), _direction(direction) {
    }

    void set(const Shader& shader, const char* prefixUniformName) const override;
private:
    glm::vec3 _direction;
};

class PointLight : public Light {
public:
    PointLight() = delete;
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity, float constant, float linear, float quadratic) : Light(ambient, diffuse, specular, intensity), _position(position), _constant(constant), _linear(linear), _quadratic(quadratic) {
    }
    void set(const Shader& shader, const char* prefixUniformName) const override;
private:
    glm::vec3 _position;
    float _constant;
    float _linear;
    float _quadratic;
};

#endif // light_hpp