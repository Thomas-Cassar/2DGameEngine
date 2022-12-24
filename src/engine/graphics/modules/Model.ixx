module;
#include "glm/glm.hpp"
#include <assimp/scene.h>
#include <filesystem>
#include <functional>
#include <vector>
export module Graphics:Model;
import :Mesh;
import :Resource;

namespace Graphics
{

export using ForEachMeshFn = std::function<void(Mesh&)>;

export class Model {
public:
    Model(std::filesystem::path const& modelPath, ResourceManager& resourceManager);

    void forEachMesh(ForEachMeshFn const& fn);

private:
    void loadSceneNode(aiScene const* scene, aiNode* node, glm::mat4 transform, ResourceManager& resourceManager,
                       std::filesystem::path const& modelPath);

    std::vector<Mesh> meshes;
};
} // namespace Graphics
