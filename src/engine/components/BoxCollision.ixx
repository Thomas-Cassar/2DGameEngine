module;
#include "glm/glm.hpp"
export module Component:BoxCollision;

import Ecs;

namespace Component
{
export struct BoxCollision : public Ecs::IComponent {

    float width;             /// Local X axis
    float height;            /// Local Y axis
    float depth;             /// Local Z axis
    bool canOverlap = false; /// True if we should not resolve collisions with this box
    bool colliding;          /// True if overlapping with another box collision this frame

    /// If a collision occurred on the this frame and was resolved. This tells us the axes of collision.
    struct {
        bool x;
        bool y;
        bool z;
    } collisionAxis;

    BoxCollision(float width, float height, float depth) : width(width), height(height), depth(depth), colliding(false)
    {
    }
};
} // namespace Component