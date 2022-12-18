module;
#include <memory>
export module Component:MeshComponent;
import Ecs;
import Graphics;

namespace Component
{

export struct MeshComponent : public Ecs::IComponent {
    std::unique_ptr<Graphics::VertexBuffer> vertexBuffer;
    std::unique_ptr<Graphics::VertexBufferLayout> vertexBufferLayout;
    std::unique_ptr<Graphics::VertexArray> vertexArray;
    std::unique_ptr<Graphics::IndexBuffer> indexBuffer;
    std::unique_ptr<Graphics::Shader> shader;

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
} // namespace Component
