#ifndef geometry_hpp
#define geometry_hpp

#include <cstdint>

// Attribute locations
// Attribute 0 = Position
// Attribute 1 = UV
// Attribute 2 = Normal
// Attribute 3 = Tangent
// Attribute 4 = Bitangent

class Geometry {
    public:
        // Constructors
        Geometry() = default;
        virtual ~Geometry();
        // Copy and move constructors
        Geometry(const Geometry&) = default;
        Geometry(Geometry&&) = default;
        // Copy and move assignment operators
        Geometry& operator=(const Geometry&) = default;
        Geometry& operator=(Geometry&&) = default;

        // Methods
        virtual void render() const;
    protected:
        void uploadData(const float* positions, const float* uvs, const float* normals, const uint32_t* indices);

        // Attributes
        uint32_t _VAO = 0;
        uint32_t _VBO[6] = {0, 0, 0, 0, 0, 0};
        uint32_t _nVertices = 0;
        uint32_t _nElements = 0;

    private:
        // Methods
        void calcTangents(const float* positions, const float* uvs, const float* normals, float* tangents, float* bitangents) const;
};



#endif /* geometry_hpp */