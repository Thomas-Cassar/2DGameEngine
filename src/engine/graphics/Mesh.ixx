module;
#include "glm/glm.hpp"
#include <assimp/scene.h>
#include <filesystem>
#include <memory>
#include <vector>
export module Graphics:Mesh;
import :IndexBuffer;
import :Shader;
import :VertexArray;
import :VertexBuffer;
import :VertexBufferLayout;
import :Texture2D;
import :ResourceManager;

namespace Graphics
{
export struct Mesh {
    Mesh(aiScene const* scene, aiMesh const* mesh, glm::mat4 const& localModelMatrix, ResourceManager& resourceManager,
         std::filesystem::path modelPath);

    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<VertexBufferLayout> vertexBufferLayout;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<Shader> shader;
    std::vector<ResourceId> baseColorTextures;
    glm::mat4 localModelMatrix;
};
} // namespace Graphics
