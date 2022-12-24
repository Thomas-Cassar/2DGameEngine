module;
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.hpp"
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
module Graphics:Texture2D;
import "graphics/Common.hpp";
import :ResourceManager;

using namespace std::string_literals;

namespace Graphics
{
Texture2D::Texture2D(std::filesystem::path const& filePath, ResourceManager& resourceManager)
{
    // Load image with stb
    // stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.string().c_str(), &width, &height, &numChannels, 0);
    if (data == nullptr)
    {
        throw std::exception(("Failed to load texture "s + filePath.string()).c_str());
    }
    // Create texture
    glCheck(glGenTextures(1, &texture2DID));
    glCheck(glBindTexture(GL_TEXTURE_2D, texture2DID));

    // Image wrapping and filtering options
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    // Send over image data
    glCheck(glTexImage2D(GL_TEXTURE_2D, 0, numChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0,
                         numChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data));

    glCheck(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(data);
    unbind();
}

Texture2D::Texture2D(Texture2D&& other)
{
    if (this != &other)
    {
        texture2DID = other.texture2DID;
        numChannels = other.numChannels;
        width = other.width;
        height = other.height;
    }
    other.texture2DID = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other)
{
    if (this != &other)
    {
        texture2DID = other.texture2DID;
        numChannels = other.numChannels;
        width = other.width;
        height = other.height;
    }
    other.texture2DID = 0;
    return *this;
}

Texture2D::~Texture2D() { glCheck(glDeleteTextures(1, &texture2DID)); }

void Texture2D::bind(unsigned int textureUnit)
{
    glCheck(glActiveTexture(GL_TEXTURE0 + textureUnit));
    glCheck(glBindTexture(GL_TEXTURE_2D, texture2DID));
}

void Texture2D::unbind() { glCheck(glBindTexture(GL_TEXTURE_2D, 0)); }
} // namespace Graphics
