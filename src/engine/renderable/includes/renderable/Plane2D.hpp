#pragma once
#include "renderable/BaseMesh.hpp"
#include "graphics/Texture2D.hpp"
#include <string>

class Plane2D : public BaseMesh
{
    private:
    float width=1.0f;
    float height=1.0f;
    Texture2D *tex=nullptr;

    public:
    Plane2D(bool doesTick,float width,float height,glm::vec3 color);
    Plane2D(bool doesTick,float width,float height,const std::string &texture);
    ~Plane2D();
    void render(glm::mat4 projectionViewMatrix);
};
