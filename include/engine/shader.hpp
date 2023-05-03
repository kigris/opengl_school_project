#ifndef shader_hpp
#define shader_hpp

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
#include <glm/glm.hpp>
#ifdef _WIN32 // If OS is Windows
#pragma warning(pop)
#else // If OS is MacOS or Linux
#pragma clang diagnostic pop
#endif

#include <string>

class Shader {
    public:
        enum class Type {
            Vertex = 0,
            Fragment = 1,
            Geometry = 2,
            Program = 3
        };
    
        Shader() = delete;
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
        ~Shader();

        Shader(const Shader&) = default; // Copy constructor
        Shader(Shader&&) = default; // Move constructor
        Shader& operator=(const Shader&) = default; // Copy assignment
        Shader& operator=(Shader&&) = default; // Move assignment

        void use() const;

        void set(const char* name, bool value) const;
        void set(const char* name, int value) const;
        void set(const char* name, float value) const;
        void set(const char* name, float value1, float value2) const;
        void set(const char* name, float value1, float value2, float value3) const;
        void set(const char* name, float value1, float value2, float value3, float value4) const;
        void set(const char* name, const glm::vec2& value) const;
        void set(const char* name, const glm::vec3& value) const;
        void set(const char* name, const glm::vec4& value) const;
        void set(const char* name, const glm::mat2& value) const;
        void set(const char* name, const glm::mat3& value) const;
        void set(const char* name, const glm::mat4& value) const;
    private:
        static void checkErrors(uint32_t shader, Type type);
        static void loadShader(const char* path, std::string* code);
        uint32_t _id;    
};

#endif /* shader_hpp */