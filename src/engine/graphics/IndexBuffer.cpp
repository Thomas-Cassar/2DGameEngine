#include "graphics/IndexBuffer.hpp"
#include "graphics/Graphics.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :count(count)
{
    glCheck(glGenBuffers(1,&ibID));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, ibID));
	glCheck(glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    glCheck(glDeleteBuffers(1,&ibID));
}

void IndexBuffer::bind() const
{
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibID));
}

void IndexBuffer::unbind() const
{
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::getCount() const
{
    return count;
}
