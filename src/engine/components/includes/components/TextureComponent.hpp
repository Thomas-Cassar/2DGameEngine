#pragma once

#include "components/IComponent.hpp"
#include "graphics/Texture2D.hpp"

#include <memory>

struct TextureComponent : public IComponent {
    std::unique_ptr<Texture2D> tex;

    TextureComponent(std::string const& texture) : tex(std::make_unique<Texture2D>(texture)) {}

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