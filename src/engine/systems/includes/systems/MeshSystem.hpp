#pragma once

#include "ecs/EntityManager.hpp"
#include "ecs/ISystem.hpp"
#include "glm/glm.hpp"

import Component;

class MeshSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;
    static Entity createCubeColored(EntityManager& manager, Component::TransformComponent&& pos,
                                    glm::vec4&& color = {1.0F, 1.0F, 1.0F, 1.0F});
    static Entity createCubeTextured(EntityManager& manager, Component::TransformComponent&& pos,
                                     std::string const& texture);
};