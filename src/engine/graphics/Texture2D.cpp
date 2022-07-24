#include "graphics/Texture2D.hpp"
#include "graphics/Graphics.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.hpp"

Texture2D::Texture2D(const std::string& filePath)
{
    //Load image with stb
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data = stbi_load(filePath.c_str(),&width,&height,&numChannels,0);
    if(data==nullptr)
    {
        std::cerr<<"Failed to load texture "<<filePath<<std::endl;
        engineAssert(false);
    }
    //Create texture
    glCheck(glGenTextures(1,&texture2DID));
    glCheck(glBindTexture(GL_TEXTURE_2D, texture2DID));

    //Image wrapping and filtering options
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    //Send over image data
    glCheck(glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data));

    glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    
    stbi_image_free(data);
}

Texture2D::~Texture2D()
{
    glCheck(glDeleteTextures(1,&texture2DID));
}

void Texture2D::bind()
{
    glCheck(glActiveTexture(GL_TEXTURE0));
    glCheck(glBindTexture(GL_TEXTURE_2D, texture2DID));
}

void Texture2D::unbind()
{
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}