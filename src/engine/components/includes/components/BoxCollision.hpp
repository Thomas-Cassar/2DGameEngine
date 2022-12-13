#pragma once
#include "components/IComponent.hpp"
#include "glm/glm.hpp"

struct BoxCollision : public IComponent {

    float width;    /// Local X axis
    float height;   /// Local Y axis
    float depth;    /// Local Z axis
    bool colliding; /// True if overlapping with another box collision this frame

    BoxCollision(float width, float height, float depth) : width(width), height(height), depth(depth), colliding(false)
    {
    }
};