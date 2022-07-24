#pragma once
#include "entity_component/Entity.hpp"
#include "glm/glm.hpp"

class Actor : public Entity
{
public:
    Actor();
    glm::vec3 getTranslation();
    void setTranslation(glm::vec3 translation);
    glm::vec3 getRotation();
    void setRotation(glm::vec3 rotation);
    float getPitch();
    void setPitch(float pitch);
    float getYaw();
    void setYaw(float yaw);
    float getRoll();
    void setRoll(float roll);
    glm::mat4 getModelMatrix();

private:
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    /** Pitch, yaw, roll */
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
};