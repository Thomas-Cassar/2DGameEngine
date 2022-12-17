#include "gl/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "systems/MeshSystem.hpp"
import Graphics;

void MeshSystem::update(EntityManager& manager, float deltaTime_s)
{
    // Get projection*view matrix from the first camera available
    glm::mat4 projectionViewMatrix;
    ComponentsForEachFn<Component::CameraComponent, Component::TransformComponent> const forEachCamera{
        [&manager, &projectionViewMatrix](Entity entity, Component::CameraComponent& cam,
                                          Component::TransformComponent& transform) {
            projectionViewMatrix =
                cam.proj * glm::lookAt(transform.position + cam.offset,
                                       cam.cameraFront + transform.position + cam.offset, cam.cameraUp);
            return false;
        }};
    manager.forEachComponents(forEachCamera);

    // Render every mesh
    ComponentsForEachFn<Component::MeshComponent, Component::TransformComponent> const forEachMesh{
        [&manager, &projectionViewMatrix](Entity entity, Component::MeshComponent& component,
                                          Component::TransformComponent& transform) {
            if (component.vertexBuffer == nullptr || component.vertexBufferLayout == nullptr ||
                component.vertexArray == nullptr || component.indexBuffer == nullptr || component.shader == nullptr)
            {
                return true;
            }
            component.shader->bind();
            if (manager.hasComponents<Component::ColorComponent>(entity))
            {
                glm::vec4& color{manager.getComponent<Component::ColorComponent>(entity).color};
                component.shader->setUniform4f("COLOR", color.x, color.y, color.z, color.w);
            }
            glm::mat4 modelMat{glm::translate(glm::mat4(1.0F), transform.position)};
            modelMat *= glm::toMat4(transform.rotation);
            modelMat = glm::scale(modelMat, transform.scale);
            glm::mat4 mvp = projectionViewMatrix * modelMat;
            component.shader->SetUniformMat4f("MVP", mvp);

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

// clang-format off
constexpr unsigned int cubeIndicies[] = {
        0,1,2,// Bottom face
        0,2,3,
        4,6,5,// Top face
        4,7,6,
        3,2,7,// Front face
        2,6,7,
        2,1,6,// Right face
        1,5,6,
        1,0,5,// Back face
        0,4,5,
        0,3,7,// Left face
        0,7,4
};

// clang-format on

Entity MeshSystem::createCubeColored(EntityManager& manager, Component::TransformComponent&& pos, glm::vec4&& color)
{
    Entity entity{manager.createEntity()};
    // clang-format off
    float const verArray[] = {// Translation
                                -1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,
                                 1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,
                                 1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,
                                -1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,
                                -1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,
                                 1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,
                                 1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,
                                -1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,
    };
    // clang-format on
    Component::MeshComponent mesh;
    mesh.vertexBuffer = std::make_unique<Graphics::VertexBuffer>(verArray, static_cast<unsigned int>(sizeof(verArray)));

    mesh.vertexBufferLayout = std::make_unique<Graphics::VertexBufferLayout>();
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

Entity MeshSystem::createCubeTextured(EntityManager& manager, Component::TransformComponent&& pos,
                                      std::string const& texture)
{
    Entity entity{manager.createEntity()};
    // clang-format off
    float const verArray[] = {// Translation                            Texture Coords
                                -1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,   1.0,0.0,
                                 1.0f / 2.0f, -1.0f / 2.0f, -1.0f/2.0f,   0.0,0.0,
                                 1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,   1.0,0.0,
                                -1.0f / 2.0f, -1.0f / 2.0f,  1.0f/2.0f,   0.0,0.0,
                                -1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,   1.0,1.0,
                                 1.0f / 2.0f,  1.0f / 2.0f, -1.0f/2.0f,   0.0,1.0,
                                 1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,   1.0,1.0,
                                -1.0f / 2.0f,  1.0f / 2.0f,  1.0f/2.0f,   0.0,1.0,
    };
    // clang-format on
    Component::MeshComponent mesh;
    mesh.vertexBuffer = std::make_unique<Graphics::VertexBuffer>(verArray, static_cast<unsigned int>(sizeof(verArray)));

    mesh.vertexBufferLayout = std::make_unique<Graphics::VertexBufferLayout>();
    mesh.vertexBufferLayout->push(GL_FLOAT, 3, false);
    mesh.vertexBufferLayout->push(GL_FLOAT, 2, false);

    mesh.vertexArray = std::make_unique<Graphics::VertexArray>();
    mesh.vertexArray->addBuffer(*mesh.vertexBuffer, *mesh.vertexBufferLayout);

    mesh.indexBuffer = std::make_unique<Graphics::IndexBuffer>(
        cubeIndicies, static_cast<unsigned int>(sizeof(cubeIndicies) / sizeof(cubeIndicies[0])));

    mesh.shader = std::make_unique<Graphics::Shader>("shaders/Plane2DTextured.vert", "shaders/Plane2DTextured.frag");
    mesh.shader->bind();
    mesh.shader->setUniform1i("texture1", 0);
    manager.addComponent<Component::TextureComponent>(entity, {texture});

    manager.addComponent<Component::MeshComponent>(entity, std::move(mesh));
    manager.addComponent<Component::TransformComponent>(entity, std::move(pos));
    return entity;
}