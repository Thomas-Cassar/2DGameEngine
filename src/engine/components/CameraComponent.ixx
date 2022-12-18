module;
#include "glm/glm.hpp"
export module Component:CameraComponent;

import Ecs;

namespace Component
{

export struct CameraComponent : public Ecs::IComponent {
    /// Point that camera is looking at (normalized)
    glm::vec3 cameraFront;
    /// Vector that points to up direction
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    /// Camera projection matrix
    glm::mat4 proj;
    /// Camera offset from its position
    glm::vec3 offset = {};
    /// Yaw value used for the cameraFront vector and the transforms rotation
    float targetYaw_rad = {};
    /// Pitch value used for the cameraFront vector
    float targetPitch_rad = {};

    CameraComponent(glm::vec3 offset) : offset(offset) {}
};
} // namespace Component
