module;
#include "gl/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
export module System:MeshSystem;
import Graphics;
import Ecs;
import Component;
// Using import here to prevent a compiler error
import <memory>;
import <string>;

namespace System
{

// clang-format off
constexpr unsigned int cubeIndicies[] = {
        0,1,2,// -Y
        0,2,3,
        4,6,5,// +Y
        4,7,6,
        9,11,8,// +Z
        11,10,8,
        12,15,14,// +X
        12,13,15,
        16,19,18,// -Z
        16,17,19,
        20,22,23,// -X
        20,21,22
};
        // clang-format off
constexpr float cubeVerticies[] = {// Translation                            // Normals
                            // -y
                                -1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,    0.0f, -1.0f, 0.0f,
                                 1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,    0.0f, -1.0f, 0.0f,
                                 1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,    0.0f, -1.0f, 0.0f,
                                -1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,    0.0f, -1.0f, 0.0f,
                            // +Y
                                -1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,    0.0f, 1.0f, 0.0f,
                                 1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,    0.0f, 1.0f, 0.0f,
                                 1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,    0.0f, 1.0f, 0.0f,
                                -1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,    0.0f, 1.0f, 0.0f,
                            // +Z
                               -1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,     0.0f,0.0f,1.0f,
                                1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,     0.0f,0.0f,1.0f,
                               -1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,     0.0f,0.0f,1.0f,
                                1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,     0.0f,0.0f,1.0f,
                            // +X
                                1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,     1.0f,0.0f,0.0f,
                                1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,     1.0f,0.0f,0.0f,
                                1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,     1.0f,0.0f,0.0f,
                                1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,     1.0f,0.0f,0.0f,
                            // -Z
                                1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,     0.0f,0.0f,-1.0f,
                               -1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,     0.0f,0.0f,-1.0f,
                                1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,     0.0f,0.0f,-1.0f,
                               -1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,     0.0f,0.0f,-1.0f,
                            // -X
                               -1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,     -1.0f,0.0f,0.0f,
                               -1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,     -1.0f,0.0f,0.0f,
                               -1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,     -1.0f,0.0f,0.0f,
                               -1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,     -1.0f,0.0f,0.0f,

};

// clang-format on

export class MeshSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        // Get projection*view matrix from the first camera available
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec3 camPos;
        Ecs::ComponentsForEachFn<Component::CameraComponent, Component::TransformComponent> const forEachCamera{
            [&projection,&view,&camPos](Ecs::Entity entity, Component::CameraComponent& cam,
                                    Component::TransformComponent& transform) {
                projection = cam.proj;
                view = glm::lookAt(transform.position + cam.offset, cam.cameraFront + transform.position + cam.offset,
                                   cam.cameraUp);
                camPos = transform.position;
                return false;
            }};
        manager.forEachComponents(forEachCamera);

        glm::vec3 ambientLightColor{};
        Ecs::ComponentsForEachFn<Component::AmbientLightComponent> const forEachAmbientLight{
            [&ambientLightColor](Ecs::Entity entity, Component::AmbientLightComponent& light) {
                ambientLightColor += light.strength * light.color;
                return true;
            }};
        manager.forEachComponents(forEachAmbientLight);

        // For now only support one point light
        glm::vec3 pointLightPos{};
        glm::vec3 pointLightColor{1.0f, 1.0f, 1.0f};
        float pointLightSpecularStength{};
        Ecs::ComponentsForEachFn<Component::PointLightComponent, Component::TransformComponent> const forEachPointLight{
            [&pointLightPos, &pointLightColor, &pointLightSpecularStength](
                Ecs::Entity entity,
                                                                   Component::PointLightComponent& light,
                                                                   Component::TransformComponent& transform) {
                pointLightColor = light.diffuseStrength * light.color;
                pointLightPos = transform.position;
                pointLightSpecularStength = light.specularStength;

                return false;
            }};
        manager.forEachComponents(forEachPointLight);

        // Render every mesh
        Ecs::ComponentsForEachFn<Component::MeshComponent, Component::TransformComponent> const forEachMesh{
            [&manager, &view, &projection, &ambientLightColor, &pointLightPos, &pointLightColor, &camPos,
             &pointLightSpecularStength](
                Ecs::Entity entity, Component::MeshComponent& component, Component::TransformComponent& transform) {
                if (!component.vertexBuffer || !component.vertexBufferLayout || !component.vertexArray ||
                    !component.indexBuffer || !component.shader)
                {
                    return true;
                }
                component.shader->bind();
                if (manager.hasComponents<Component::ColorComponent>(entity))
                {
                    glm::vec3& color{manager.getComponent<Component::ColorComponent>(entity).color};
                    component.shader->setUniform3f("u_objectColor", color.x, color.y, color.z);
                }
                else
                {
                    component.shader->setUniform3f("u_objectColor", 0.0f, 0.0f, 0.0f);
                }
                glm::mat4 model{glm::translate(glm::mat4(1.0F), transform.position)};
                model *= glm::toMat4(transform.rotation);
                model = glm::scale(model, transform.scale);

                component.shader->SetUniformMat4f("u_model", model);
                component.shader->SetUniformMat3f("u_normalMat", glm::transpose(glm::inverse(model)));
                component.shader->SetUniformMat4f("u_view", view);
                component.shader->SetUniformMat4f("u_projection", projection);
                component.shader->setUniform3f("u_ambientLightColor", ambientLightColor.x, ambientLightColor.y,
                                               ambientLightColor.z);
                component.shader->setUniform3f("u_pointLightPos", pointLightPos.x, pointLightPos.y, pointLightPos.z);
                component.shader->setUniform3f("u_pointLightColor", pointLightColor.x, pointLightColor.y, pointLightColor.z);
                component.shader->setUniform3f("u_viewPos", camPos.x, camPos.y, camPos.z);
                component.shader->setUniform1f("u_specularStrength", pointLightSpecularStength);

                component.vertexArray->bind();
                component.indexBuffer->bind();

                if (manager.hasComponents<Component::TextureComponent>(entity))
                {
                    manager.getComponent<Component::TextureComponent>(entity).tex->bind();
                }

                Graphics::drawBoundBuffers(component.indexBuffer->getCount());

                return true;
            }};
        manager.forEachComponents(forEachMesh);
    }

    static Ecs::Entity addCubeMeshComponent(Ecs::EntityManager& manager, Ecs::Entity entity, Component::TransformComponent&& pos,
                                         glm::vec3&& color = {1.0F, 1.0F, 1.0F})
    {
        Component::MeshComponent mesh;
        mesh.vertexBuffer =
            std::make_unique<Graphics::VertexBuffer>(cubeVerticies, static_cast<unsigned int>(sizeof(cubeVerticies)));

        mesh.vertexBufferLayout = std::make_unique<Graphics::VertexBufferLayout>();
        mesh.vertexBufferLayout->push(GL_FLOAT, 3, false);
        mesh.vertexBufferLayout->push(GL_FLOAT, 3, false);

        mesh.vertexArray = std::make_unique<Graphics::VertexArray>();
        mesh.vertexArray->addBuffer(*mesh.vertexBuffer, *mesh.vertexBufferLayout);

        mesh.indexBuffer = std::make_unique<Graphics::IndexBuffer>(
            cubeIndicies, static_cast<unsigned int>(sizeof(cubeIndicies) / sizeof(cubeIndicies[0])));

        mesh.shader = std::make_unique<Graphics::Shader>("shaders/Plane2DColored.vert", "shaders/Plane2DColored.frag");

        manager.addComponent<Component::MeshComponent>(entity, std::move(mesh));
        manager.addComponent<Component::TransformComponent>(entity, std::move(pos));
        manager.addComponent<Component::BoxCollision>(entity, {pos.scale.x, pos.scale.y, pos.scale.z});
        manager.addComponent<Component::ColorComponent>(entity, std::move(color));
        return entity;
    }
};
} // namespace System
