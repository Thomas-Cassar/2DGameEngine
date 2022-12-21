export module Graphics:VertexBuffer;

import "graphics/Common.hpp";

namespace Graphics
{

export class VertexBuffer {
private:
    /** ID given to this vertex buffer by OpenGL*/
    unsigned int vbID;

public:
    /**
     * @brief Construct a new Vertex Buffer object
     *
     * @param data Array of vertices
     * @param size Size in bytes of the vertex buffer
     * @post vbID is set to an ID given by OpenGL
     */
    VertexBuffer(const void* data, unsigned int size)
    {
        glCheck(glGenBuffers(1, &vbID));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbID));
        glCheck(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    VertexBuffer(VertexBuffer const&) = delete;

    VertexBuffer& operator=(VertexBuffer const&) = delete;

    /**
     * @brief Destroy the Vertex Buffer object
     *
     */
    ~VertexBuffer() { glCheck(glDeleteBuffers(1, &vbID)); }

    /**
     * @brief Binds this as the current VertexBuffer
     *
     */
    void bind() const { glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbID)); }

    /**
     * @brief Unbinds the current VertexBuffer
     *
     */
    void unbind() const { glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
};
} // namespace Graphics