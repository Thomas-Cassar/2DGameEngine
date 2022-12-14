#pragma once

#include "components/IComponent.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"

#include <memory>

struct MeshComponent : public IComponent {
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<VertexBufferLayout> vertexBufferLayout;
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<Shader> shader;

    MeshComponent(MeshComponent const&) = delete;
    MeshComponent& operator=(MeshComponent const&) = delete;

    MeshComponent() {}

    MeshComponent(MeshComponent&& other)
        : vertexBuffer(std::move(other.vertexBuffer)), vertexBufferLayout(std::move(other.vertexBufferLayout)),
          vertexArray(std::move(other.vertexArray)), indexBuffer(std::move(other.indexBuffer)),
          shader(std::move(other.shader))
    {
    }

    MeshComponent& operator=(MeshComponent&& other)
    {
        if (this != &other)
        {
            vertexBuffer = std::move(other.vertexBuffer);
            vertexBufferLayout = std::move(other.vertexBufferLayout);
            vertexArray = std::move(other.vertexArray);
            indexBuffer = std::move(other.indexBuffer);
            shader = std::move(other.shader);
        }
        return *this;
    }
};