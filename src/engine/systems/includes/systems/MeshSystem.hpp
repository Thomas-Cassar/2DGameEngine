#pragma once

#include "components/MeshComponent.hpp"
#include "components/TransformComponent.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/ISystem.hpp"

class MeshSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;
    static Entity createCubeColored(EntityManager& manager, TransformComponent&& pos,
                                    glm::vec4&& color = {1.0F, 1.0F, 1.0F, 1.0F});
    static Entity createCubeTextured(EntityManager& manager, TransformComponent&& pos, std::string const& texture);
};