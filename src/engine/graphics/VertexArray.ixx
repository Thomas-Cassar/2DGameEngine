export module Graphics:VertexArray;

import "graphics/Common.hpp";
import <vector>;
export import :VertexBufferLayout;
export import :VertexBuffer;

namespace Graphics
{

export class VertexArray {
private:
    /** ID given to this VA by OpenGL*/
    unsigned int vaID;

public:
    /**
     * @brief Construct a new Vertex Array object
     * @post vaID is given an ID by OpenGL
     */
    VertexArray() { glCheck(glGenVertexArrays(1, &vaID)); }

    /**
     * @brief Destroy the Vertex Array object
     *
     */
    ~VertexArray() { glCheck(glDeleteVertexArrays(1, &vaID)); }

    /**
     * @brief Adds a vertexbuffer to this vertex array
     *
     * @param vb The vertex buffer to add
     * @param layout The layout of the vertex buffer
     */
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
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

    /**
     * @brief Binds this as the current VA
     *
     */
    void bind() const { glCheck(glBindVertexArray(vaID)); }

    /**
     * @brief Unbinds the current VA
     *
     */
    void unbind() const { glCheck(glBindVertexArray(0)); }
};
} // namespace Graphics