#include "object/Actor.hpp"
#include "glm/gtc/matrix_transform.hpp"

Actor::Actor(bool doesTick)
:Object(doesTick)
{

}

void Actor::objectTick()
{
    
}

glm::vec3 Actor::getTranslation()
{
    return translation;
}
void Actor::setTranslation(glm::vec3 translation)
{
    this->translation=translation;
}
glm::vec3 Actor::getRotation()
{
    return rotation;
}
void Actor::setRotation(glm::vec3 rotation)
{
    this->rotation=rotation;
}

float Actor::getPitch()
{
    return rotation.x;
}
void Actor::setPitch(float pitch)
{
    rotation.x=pitch;
}
float Actor::getYaw()
{
    return rotation.y;
}
void Actor::setYaw(float yaw)
{
    rotation.y=yaw;
}
float Actor::getRoll()
{
    return rotation.z;
}
void Actor::setRoll(float roll)
{
    rotation.z=roll;
}

glm::mat4 Actor::getModelMatrix()
{
    glm::mat4 mod=glm::translate(glm::mat4(1.0f),translation);
    mod=glm::rotate(mod,glm::radians(getYaw()),glm::vec3(0.0f,1.0f,0.0f));
    mod=glm::rotate(mod,glm::radians(getPitch()),glm::vec3(1.0f,0.0f,0.0f));
    mod=glm::rotate(mod,glm::radians(getRoll()),glm::vec3(0.0f,0.0f,1.0f));

    return mod;
}