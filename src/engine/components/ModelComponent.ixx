module;
#include <utility>
export module Component:ModelComponent;
import Ecs;
import Core;
import Graphics;

namespace Component
{

export struct ModelComponent : public Ecs::IComponent {
    Graphics::ResourceId modelID;
    Graphics::ResourceManager* resources;

    ModelComponent(Graphics::ResourceId modelID, Graphics::ResourceManager* resources)
        : modelID(modelID), resources(resources)
    {
    }
};
} // namespace Component
