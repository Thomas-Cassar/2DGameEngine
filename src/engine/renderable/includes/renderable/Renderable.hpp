#pragma once
#include "object/Actor.hpp"
#include "glm/glm.hpp"

class Renderable : public Actor 
{
    private:
    bool visible=true;
    public:
    Renderable(bool doesTick);
    virtual void render(glm::mat4 projectionViewMatrix)=0;
    void setVisible(bool visible);
    bool getVisible();
};