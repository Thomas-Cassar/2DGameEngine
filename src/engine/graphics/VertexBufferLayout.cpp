#include "VertexBufferLayout.hpp"

VertexBufferLayout::VertexBufferLayout()
:stride(0)
{}

void VertexBufferLayout::push(unsigned int type, unsigned int count, bool normalize)
{
    stride+=VertexBufferElement::getSizeOfGLType(type) * count;
    if(normalize)
        elements.push_back({type,count,GL_TRUE});
    else
        elements.push_back({type,count,GL_FALSE});
}

const std::vector<VertexBufferElement> VertexBufferLayout::getElements() const
{
    return elements;
}

unsigned int VertexBufferLayout::getStride() const
{
    return stride;
}