#include <engine/shader.hpp>

#ifdef _WIN32 // If OS is Windows
#pragma warning(push, 0)
#elif __APPLE__ // If OS is MacOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#elif __linux__ // for Linux with Clang compiler
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#endif
// Warning of includes that it is wanted to be ignored
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#ifdef _WIN32 // If OS is Windows
#pragma warning(pop)
#else // If OS is MacOS or Linux
#pragma clang diagnostic pop
#endif

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    // Variables to store the code of the shaders
    std::string sVertexCode, sFragmentCode, sGeometryCode;
    // Load the shaders and store the code in the variables
    loadShader(vertexPath, &sVertexCode);
    loadShader(fragmentPath, &sFragmentCode);
    // If geometry shader is used
    if (geometryPath)
        loadShader(geometryPath, &sGeometryCode);

    // Convert the code to a format that OpenGL can understand
    const char* vertexCode = sVertexCode.c_str();
    const char* fragmentCode = sFragmentCode.c_str();
    // Create the shaders
    const uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    checkErrors(vertex, Type::Vertex);

    const uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    checkErrors(fragment, Type::Fragment);

    uint32_t geometry;
    if(geometryPath){
        const char* geometryCode = sGeometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryCode, nullptr);
        glCompileShader(geometry);
        checkErrors(geometry, Type::Geometry);
    }

    // Create the program
    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    if(geometryPath)
        glAttachShader(_id, geometry);
    glLinkProgram(_id);
    checkErrors(_id, Type::Program);
    // Delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geometryPath)
        glDeleteShader(geometry);
}

Shader::~Shader() {
    glDeleteProgram(_id);
}

void Shader::use() const {
    glUseProgram(_id);
}

void Shader::set(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(_id, name), (int)value);
}

void Shader::set(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(_id, name), value);
}

void Shader::set(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(_id, name), value);
}

void Shader::set(const char* name, float value1, float value2) const {
    glUniform2f(glGetUniformLocation(_id, name), value1, value2);
}

void Shader::set(const char* name, float value1, float value2, float value3) const {
    glUniform3f(glGetUniformLocation(_id, name), value1, value2, value3);
}

void Shader::set(const char* name, float value1, float value2, float value3, float value4) const {
    glUniform4f(glGetUniformLocation(_id, name), value1, value2, value3, value4);
}

void Shader::set(const char* name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(_id, name), 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(_id, name), 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(_id, name), 1, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::mat2& value) const {
    glUniformMatrix2fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::mat3& value) const {
    glUniformMatrix3fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const char* name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkErrors(const uint32_t shader, const Type type) {
    int success;
    char infoLog[512];
    if(type != Type::Program) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR: Failed to compile shader of type " << (type == Type::Vertex ? "Vertex" : (type == Type::Fragment ? "Fragment" : "Geometry")) << "\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR: Failed to link program" << "\n" << infoLog << std::endl;
        }
    }
}

void Shader::loadShader(const char* path, std::string* code) {
    std::ifstream file;
    file.open(path, std::ios_base::in);
    if(file.is_open()) {
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        *code = stream.str();
    } else
        std::cerr << "ERROR: Failed to open file " << path << std::endl;
}