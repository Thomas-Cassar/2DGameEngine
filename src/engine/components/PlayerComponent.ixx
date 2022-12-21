export module Component:PlayerComponent;
import Ecs;

namespace Component
{
export struct PlayerComponent : public Ecs::IComponent {
    bool debugControls = false;

    PlayerComponent() {}
};
} // namespace Component
