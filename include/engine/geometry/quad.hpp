#pragma once

#include "engine/geometry/geometry.hpp"

class Quad final : public Geometry {
    public:
        Quad() = delete;
        explicit Quad(float size);

    private:
        [[maybe_unused]] float _size;
};