#pragma once

#include "Renderable.hpp"

#include <memory>
#include <vector>

class RenderableManager
{
public:
    void registerRenderable(std::shared_ptr<Renderable> renderable);
    void renderRenderables(glm::mat4 projectionViewMatrix);
private:
    std::vector<std::shared_ptr<Renderable>> renderables;
};