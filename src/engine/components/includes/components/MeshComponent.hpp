#pragma once

#include "components/IComponent.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"

#include <memory>

struct MeshComponent : public IComponent {
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<VertexBufferLayout> vertexBufferLayout;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<Shader> shader;
};