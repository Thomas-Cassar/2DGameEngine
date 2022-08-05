#pragma once

#include "components/MeshComponent.hpp"
#include "components/TranslationComponent.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/ISystem.hpp"

class MeshSystem : public ISystem {
public:
    void update(EntityManager& manager) override;
    static Entity createCubeColored(EntityManager& manager, TranslationComponent const& pos,
                                    glm::vec3 const& color = {1.0F, 1.0F, 1.0F});
    static Entity createCubeTextured(EntityManager& manager, TranslationComponent const& pos,
                                     std::string const& texture);
};