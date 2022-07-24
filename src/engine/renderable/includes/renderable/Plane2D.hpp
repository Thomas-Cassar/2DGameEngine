#pragma once
#include "renderable/BaseMesh.hpp"
#include "graphics/Texture2D.hpp"
#include <string>

class Plane2D : public BaseMesh
{
public:
    Plane2D(float width, float height, glm::vec3 color);
    Plane2D(float width, float height, std::string const& texture);
    ~Plane2D();
    void render(glm::mat4 projectionViewMatrix);

private:
    float width = 1.0f;
    float height = 1.0f;
    Texture2D *tex = nullptr;
};
