module;
#include <exception>
#include <string>
#include <vector>
module Graphics:VertexBufferLayout;
import "graphics/Common.hpp";

namespace Graphics
{

unsigned int VertexBufferElement::getSizeOfGLType(unsigned int type)
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

VertexBufferLayout::VertexBufferLayout() : stride(0) {}

void VertexBufferLayout::push(unsigned int type, unsigned int count, bool normalize)
{
    stride += VertexBufferElement::getSizeOfGLType(type) * count;
    if (normalize)
        elements.push_back({type, count, GL_TRUE});
    else
        elements.push_back({type, count, GL_FALSE});
}

const std::vector<VertexBufferElement> VertexBufferLayout::getElements() const { return elements; }

unsigned int VertexBufferLayout::getStride() const { return stride; }
} // namespace Graphics