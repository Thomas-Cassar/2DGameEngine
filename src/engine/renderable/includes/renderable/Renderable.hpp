#pragma once
#include "entity_component/Actor.hpp"
#include "glm/glm.hpp"

class RenderableManager;

class Renderable : public Actor {
public:
    virtual void render(glm::mat4 projectionViewMatrix) = 0;
    void setVisible(bool visible);
    bool getVisible();

private:
    bool visible = true;
};