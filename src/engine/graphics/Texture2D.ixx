module;
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.hpp"
#include <exception>
#include <string>
export module Graphics:Texture2D;

import "graphics/Common.hpp";

using namespace std::string_literals;

namespace Graphics
{
export class Texture2D {
private:
    /**ID given to the texture2D by OpenGL**/
    unsigned int texture2DID;
    /**Width of the loaded image in pixels**/
    int width;
    /**Height of the loaded image in pixels**/
    int height;
    /**Number of color channels**/
    int numChannels;

public:
    /**
     * @brief Construct a new Texture2D object
     *
     * @param filePath The filepath to the texture
     */
    Texture2D(const std::string& filePath)
    {
        // Load image with stb
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);
        if (data == nullptr)
        {
            throw std::exception(("Failed to load texture "s + filePath).c_str());
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
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));

        glCheck(glGenerateMipmap(GL_TEXTURE_2D));

        stbi_image_free(data);
    }

    /**
     * @brief Destroy the Texture2D object
     */
    ~Texture2D() { glCheck(glDeleteTextures(1, &texture2DID)); }

    /**
     * @brief Binds this as the current Texture2D
     */
    void bind()
    {
        glCheck(glActiveTexture(GL_TEXTURE0));
        glCheck(glBindTexture(GL_TEXTURE_2D, texture2DID));
    }

    /**
     * @brief Unbinds the current Texture2D
     */
    void unbind() { glCheck(glBindTexture(GL_TEXTURE_2D, 0)); }
};
} // namespace Graphics
