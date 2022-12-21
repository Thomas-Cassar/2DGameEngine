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
import <iostream>;

namespace System
{
export class MeshSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        // Get projection*view matrix from the first camera available
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec3 camPos;
        Ecs::ComponentsForEachFn<Component::CameraComponent, Component::TransformComponent> const forEachCamera{
            [&projection, &view, &camPos](Ecs::Entity entity, Component::CameraComponent& cam,
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
                Ecs::Entity entity, Component::PointLightComponent& light, Component::TransformComponent& transform) {
                pointLightColor = light.diffuseStrength * light.color;
                pointLightPos = transform.position;
                pointLightSpecularStength = light.specularStength;

                return false;
            }};
        manager.forEachComponents(forEachPointLight);

        // Render every model
        Ecs::ComponentsForEachFn<Component::ModelComponent, Component::TransformComponent> const forEachModel{
            [&view, &projection, &ambientLightColor, &pointLightPos, &pointLightColor, &camPos,
             &pointLightSpecularStength](Ecs::Entity entity, Component::ModelComponent& modelComponent,
                                         Component::TransformComponent& transform) {
                Graphics::ResourceManager* resourceManager{modelComponent.resources};
                if (resourceManager == nullptr)
                {
                    std::cerr << "Model component does not have a resource manager set" << std::endl;
                    return true;
                }
                Graphics::Model& model{resourceManager->getResource<Graphics::Model>(modelComponent.modelID)};

                Graphics::ForEachMeshFn const forEachMesh{[&view, &projection, &ambientLightColor, &pointLightPos,
                                                           &pointLightColor, &camPos, &pointLightSpecularStength,
                                                           &transform, &resourceManager](Graphics::Mesh& mesh) {
                    mesh.shader->bind();

                    glm::mat4 model{glm::translate(glm::mat4(1.0F), transform.position)};
                    model *= glm::toMat4(transform.rotation);
                    model = glm::scale(model, transform.scale);

                    mesh.shader->SetUniformMat4f("u_model", model * mesh.localModelMatrix);
                    mesh.shader->SetUniformMat3f("u_normalMat", glm::transpose(glm::inverse(model)));
                    mesh.shader->SetUniformMat4f("u_view", view);
                    mesh.shader->SetUniformMat4f("u_projection", projection);
                    mesh.shader->setUniform3f("u_ambientLightColor", ambientLightColor.x, ambientLightColor.y,
                                              ambientLightColor.z);
                    mesh.shader->setUniform3f("u_pointLightPos", pointLightPos.x, pointLightPos.y, pointLightPos.z);
                    mesh.shader->setUniform3f("u_pointLightColor", pointLightColor.x, pointLightColor.y,
                                              pointLightColor.z);
                    mesh.shader->setUniform3f("u_viewPos", camPos.x, camPos.y, camPos.z);
                    mesh.shader->setUniform1f("u_specularStrength", pointLightSpecularStength);

                    if (!mesh.baseColorTextures.empty())
                    {
                        mesh.shader->setUniform1i("u_baseColorTexture", 0);
                        Graphics::Texture2D& baseColorTexture{
                            resourceManager->getResource<Graphics::Texture2D>(mesh.baseColorTextures.at(0))};
                        baseColorTexture.bind(0);
                    }

                    mesh.vertexArray->bind();
                    mesh.indexBuffer->bind();

                    Graphics::drawBoundBuffers(mesh.indexBuffer->getCount());
                }};
                model.forEachMesh(forEachMesh);

                return true;
            }};
        manager.forEachComponents(forEachModel);
    }
};
} // namespace System
