module;
#include "glm/glm.hpp"
#include <assimp/scene.h>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>
module Graphics:Mesh;
import "graphics/Common.hpp";
import :Mesh;
import :ResourceManager;
import :Texture2D;

namespace Graphics
{
std::vector<ResourceId> loadTexturesFromMaterial(aiMaterial const* material, aiTextureType type,
                                                 ResourceManager& resourceManager,
                                                 std::filesystem::path const& modelPath)
{
    std::vector<ResourceId> result;
    for (unsigned int i{}; i < material->GetTextureCount(type); i++)
    {
        aiString fileName;
        material->GetTexture(type, i, &fileName);
        // We must append the filename to the folder that the model was found in since
        // the model file only knows the texture filename
        auto texturePath{modelPath.parent_path() / std::filesystem::path(fileName.C_Str())};
        result.emplace_back(resourceManager.getResourceId<Texture2D>(texturePath.string().c_str()));
    }

    return result;
}

Mesh::Mesh(aiScene const* scene, aiMesh const* mesh, glm::mat4 const& localModelMatrix,
                  ResourceManager& resourceManager, std::filesystem::path modelPath)
    : localModelMatrix(localModelMatrix),
      baseColorTextures(loadTexturesFromMaterial(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_BASE_COLOR,
                                                 resourceManager, modelPath))
{
    vertexBufferLayout = std::make_shared<VertexBufferLayout>();
    vertexBufferLayout->push(GL_FLOAT, 3, false); // Position
    vertexBufferLayout->push(GL_FLOAT, 3, false); // Normal
    unsigned int vertexAttribCount;
    if (mesh->mTextureCoords)
    {
        vertexBufferLayout->push(GL_FLOAT, 2, false); // Texture coordinate
        vertexAttribCount = 8U;
    }
    else
    {
        vertexAttribCount = 6U;
    }

    std::vector<float> verticies;
    verticies.reserve(mesh->mNumVertices * vertexAttribCount);
    for (unsigned int i{}; i < mesh->mNumVertices; i++)
    {
        verticies.emplace_back(mesh->mVertices[i].x);
        verticies.emplace_back(mesh->mVertices[i].y);
        verticies.emplace_back(mesh->mVertices[i].z);
        verticies.emplace_back(mesh->mNormals[i].x);
        verticies.emplace_back(mesh->mNormals[i].y);
        verticies.emplace_back(mesh->mNormals[i].z);
        if (mesh->mTextureCoords)
        {
            verticies.emplace_back(mesh->mTextureCoords[0][i].x);
            verticies.emplace_back(mesh->mTextureCoords[0][i].y);
        }
    }
    vertexBuffer = std::make_shared<VertexBuffer>(
        verticies.data(), static_cast<unsigned int>(mesh->mNumVertices * vertexAttribCount * sizeof(float)));
    vertexArray = std::make_shared<VertexArray>();
    vertexArray->addBuffer(*vertexBuffer, *vertexBufferLayout);

    std::vector<unsigned int> indicies;
    for (unsigned int i{}; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j{}; j < mesh->mFaces[i].mNumIndices; j++)
        {
            indicies.emplace_back(mesh->mFaces[i].mIndices[j]);
        }
    }
    indexBuffer = std::make_shared<IndexBuffer>(indicies.data(), static_cast<unsigned int>(indicies.size()));
    // TODO: Allow configurable shader
    shader = std::make_shared<Graphics::Shader>("shaders/Mesh.vert", "shaders/Mesh.frag");
}
} // namespace Graphics