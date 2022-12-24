module;
#include <filesystem>
module Graphics:ResourceManager;

namespace Graphics
{
template <>
ResourceId ResourceManager::getResourceId<Model>(std::filesystem::path const& path)
{
    return models.getResourceID(path, *this);
}

template <>
ResourceId ResourceManager::getResourceId<Texture2D>(std::filesystem::path const& path)
{
    return textures.getResourceID(path, *this);
}

template <>
Model& ResourceManager::getResource<Model>(ResourceId id)
{
    return models.getResource(id);
}

template <>
Texture2D& ResourceManager::getResource<Texture2D>(ResourceId id)
{
    return textures.getResource(id);
}

} // namespace Graphics
