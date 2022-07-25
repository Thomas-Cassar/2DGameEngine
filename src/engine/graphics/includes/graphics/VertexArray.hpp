#pragma once

#include "graphics/VertexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"

class VertexArray {
private:
    /** ID given to this VA by OpenGL*/
    unsigned int vaID;

public:
    /**
     * @brief Construct a new Vertex Array object
     * @post vaID is given an ID by OpenGL
     */
    VertexArray();

    /**
     * @brief Destroy the Vertex Array object
     *
     */
    ~VertexArray();

    /**
     * @brief Adds a vertexbuffer to this vertex array
     *
     * @param vb The vertex buffer to add
     * @param layout The layout of the vertex buffer
     */
    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

    /**
     * @brief Binds this as the current VA
     *
     */
    void bind() const;

    /**
     * @brief Unbinds the current VA
     *
     */
    void unbind() const;
};
