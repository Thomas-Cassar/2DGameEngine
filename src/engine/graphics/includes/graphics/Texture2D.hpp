#pragma once
#include <string>

class Texture2D {
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
    Texture2D(const std::string& filePath);
    /**
     * @brief Destroy the Texture2D object
     */
    ~Texture2D();
    /**
     * @brief Binds this as the current Texture2D
     */
    void bind();
    /**
     * @brief Unbinds the current Texture2D
     */
    void unbind();

};