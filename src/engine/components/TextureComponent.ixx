export module Component:TextureComponent;

import Ecs;
import Graphics;
import <memory>;
import <string>;

namespace Component
{
export struct TextureComponent : public Ecs::IComponent {
    std::unique_ptr<Graphics::Texture2D> tex;

    TextureComponent(std::string const& texture) : tex(std::make_unique<Graphics::Texture2D>(texture)) {}

    TextureComponent(TextureComponent const&) = delete;
    TextureComponent& operator=(TextureComponent const&) = delete;

    TextureComponent(TextureComponent&& other) : tex(std::move(other.tex)) {}

    TextureComponent& operator=(TextureComponent&& other)
    {
        if (this != &other)
        {
            tex = std::move(other.tex);
        }
        return *this;
    }
};
} // namespace Component
