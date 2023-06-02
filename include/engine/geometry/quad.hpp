#pragma once

#include "engine/geometry/geometry.hpp"

class Quad final : public Geometry {
    public:
        Quad() = delete;
        explicit Quad(float size);
        explicit Quad(float size, float offsetX, float offsetY, float offsetZ);

    private:
        [[maybe_unused]] float _size;
};