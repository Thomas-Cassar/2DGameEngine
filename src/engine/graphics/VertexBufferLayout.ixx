module;
#include <exception>
#include <string>
#include <vector>
export module Graphics:VertexBufferLayout;

import "graphics/Common.hpp";

namespace Graphics
{
/** Struct used to represent an element in a VertexBuffer*/
export struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    /**
     * @brief Get the Size in Bytes Of OpenGL type
     *
     * @param type Enum representing the type
     * @return unsigned int Size in bytes
     */
    static unsigned int getSizeOfGLType(unsigned int type)
    {
        switch (type)
        {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            return 1;

        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_HALF_FLOAT:
            return 2;

        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
        case GL_FIXED:
            return 4;

        case GL_DOUBLE:
            return 8;

        default:
            throw std::exception("Tried to get size of uknown GLType");
            return 0;
        }
    }
};

export class VertexBufferLayout {
private:
    /** Vector of VertexBufferElements representing the order and layout of elements in the buffer */
    std::vector<VertexBufferElement> elements;
    /** Size in bytes between attributes of each vertex in the buffer*/
    unsigned int stride;

public:
    /**
     * @brief Construct a new Vertex Buffer Layout object
     *
     */
    VertexBufferLayout() : stride(0) {}

    /**
     * @brief Push an attribute into the layout
     *
     * @param type The OpenGL type to push (GL_FLOAT, GL_UNSIGNED_INT etc.)
     * @param count The number of that type in this attribute
     * @param normalize If the data type is normalized
     */
    void push(unsigned int type, unsigned int count, bool normalize)
    {
        stride += VertexBufferElement::getSizeOfGLType(type) * count;
        if (normalize)
            elements.push_back({type, count, GL_TRUE});
        else
            elements.push_back({type, count, GL_FALSE});
    }

    /**
     * @brief Get the Elements object
     *
     * @return const std::vector<VertexBufferElement>
     */
    const std::vector<VertexBufferElement> getElements() const { return elements; }

    /**
     * @brief Get the Stride object
     *
     * @return unsigned int
     */
    unsigned int getStride() const { return stride; }
};
} // namespace Graphics