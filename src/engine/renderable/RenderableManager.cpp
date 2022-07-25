#include "renderable/RenderableManager.hpp"

void RenderableManager::registerRenderable(std::shared_ptr<Renderable> renderable)
{
    renderables.push_back(renderable);
}

void RenderableManager::renderRenderables(glm::mat4 projectionViewMatrix)
{
    for (auto &renderable : renderables)
    {
        if (renderable->getVisible())
        {
            renderable->render(projectionViewMatrix);
        }
    }
}