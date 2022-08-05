#pragma once

#include "components/IComponent.hpp"
#include "graphics/Texture2D.hpp"

#include <memory>

struct TextureComponent : public IComponent {
    std::shared_ptr<Texture2D> tex;

    TextureComponent(std::string const& texture) : tex(std::make_shared<Texture2D>(texture)) {}
};