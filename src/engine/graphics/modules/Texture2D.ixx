module;
#include <filesystem>
export module Graphics:Texture2D;
import :Resource;

namespace Graphics
{
export class Texture2D {
public:
    /**
     * @brief Construct a new Texture2D object
     *
     * @param filePath The filepath to the texture
     */
    Texture2D(std::filesystem::path const& filePath, ResourceManager& resourceManager);

    Texture2D(Texture2D const&) = delete;

    Texture2D& operator=(Texture2D const&) = delete;

    Texture2D(Texture2D&& other);

    Texture2D& operator=(Texture2D&& other);

    /**
     * @brief Destroy the Texture2D object
     */
    ~Texture2D();

    /**
     * @brief Binds this as the current Texture2D
     *
     * @param textureUnit Texture unit to bind this texture to
     */
    void bind(unsigned int textureUnit);

    /**
     * @brief Unbinds the current Texture2D
     */
    void unbind();

private:
    /**ID given to the texture2D by OpenGL**/
    unsigned int texture2DID;
    /**Width of the loaded image in pixels**/
    int width;
    /**Height of the loaded image in pixels**/
    int height;
    /**Number of color channels**/
    int numChannels;
};
} // namespace Graphics
