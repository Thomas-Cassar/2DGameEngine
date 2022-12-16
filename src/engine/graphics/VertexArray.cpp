#include "graphics/VertexArray.hpp"
#include "graphics/Graphics.hpp"

VertexArray::VertexArray() { glCheck(glGenVertexArrays(1, &vaID)); }

VertexArray::~VertexArray() { glCheck(glDeleteVertexArrays(1, &vaID)); }

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // Bind the array and buffer
    bind();
    vb.bind();
    const std::vector<VertexBufferElement> elements = layout.getElements();
    unsigned int offset = 0;
    // Loop through all elements and define the vertex attributes of the array
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        glCheck(glEnableVertexAttribArray(i));
        glCheck(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized,
                                      layout.getStride(), (const void*)offset));
        offset += elements[i].count * VertexBufferElement::getSizeOfGLType(elements[i].type);
    }
}

void VertexArray::bind() const { glCheck(glBindVertexArray(vaID)); }

void VertexArray::unbind() const { glCheck(glBindVertexArray(0)); }