#pragma once

#include "components/IComponent.hpp"
#include "glm/glm.hpp"

struct CameraComponent : public IComponent {
    /**Point that camera is looking at (normalized)*/
    glm::vec3 cameraFront;
    /**Vector that points to up direction*/
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    /**Camera projection matrix*/
    glm::mat4 proj;
};