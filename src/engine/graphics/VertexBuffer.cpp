#include "graphics/VertexBuffer.hpp"
#include "graphics/Graphics.hpp"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    glCheck(glGenBuffers(1, &vbID));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbID));
    glCheck(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { glCheck(glDeleteBuffers(1, &vbID)); }

void VertexBuffer::bind() const { glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbID)); }

void VertexBuffer::unbind() const { glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0)); }