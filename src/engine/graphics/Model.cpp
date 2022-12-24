module;
#include "glm/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>
module Graphics:Model;

using namespace std::string_literals;

namespace Graphics
{
glm::mat4 aiMat4ToGlmMat4(aiMatrix4x4 const& aiModelMatrix)
{
    glm::mat4 result;
    result[0] = {aiModelMatrix.a1, aiModelMatrix.b1, aiModelMatrix.c1, aiModelMatrix.d1};
    result[1] = {aiModelMatrix.a2, aiModelMatrix.b2, aiModelMatrix.c2, aiModelMatrix.d2};
    result[2] = {aiModelMatrix.a3, aiModelMatrix.b3, aiModelMatrix.c3, aiModelMatrix.d3};
    result[3] = {aiModelMatrix.a4, aiModelMatrix.b4, aiModelMatrix.c4, aiModelMatrix.d4};
    return result;
}

Model::Model(std::filesystem::path const& modelPath, ResourceManager& resourceManager)
{
    Assimp::Importer importer;
    aiScene const* scene{importer.ReadFile(modelPath.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs)};

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw std::exception(("Error loading model "s + std::string(importer.GetErrorString())).c_str());
    }
    meshes.reserve(scene->mNumMeshes);
    loadSceneNode(scene, scene->mRootNode, aiMat4ToGlmMat4(scene->mRootNode->mTransformation), resourceManager,
                  modelPath);
}

void Model::forEachMesh(ForEachMeshFn const& fn)
{
    for (auto& mesh : meshes)
    {
        fn(mesh);
    }
}

void Model::loadSceneNode(aiScene const* scene, aiNode* node, glm::mat4 transform, ResourceManager& resourceManager,
                          std::filesystem::path const& modelPath)
{
    if (node != scene->mRootNode)
    {
        transform *= aiMat4ToGlmMat4(node->mTransformation);
    }

    for (unsigned int i{}; i < node->mNumMeshes; i++)
    {
        aiMesh const* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(scene, mesh, transform, resourceManager, modelPath);
    }
    for (unsigned int i{}; i < node->mNumChildren; i++)
    {
        loadSceneNode(scene, node->mChildren[i], transform, resourceManager, modelPath);
    }
}
} // namespace Graphics
