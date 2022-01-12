#pragma once

class VertexBuffer
{
    private:
    /**
     * @brief ID given to this vertex buffer by OpenGL
     * 
     */
    unsigned int vbID;

    public:
    /**
     * @brief Construct a new Vertex Buffer object
     * 
     * @param data Array of verticies
     * @param size Size in bytes of the vertex buffer
     * @post vbID is set to an ID given by OpenGL
     */
    VertexBuffer(const void* data, unsigned int size);
    /**
     * @brief Destroy the Vertex Buffer object
     * 
     */
    ~VertexBuffer();

    /**
     * @brief Binds this as the current VertexBuffer
     * 
     */
    void bind() const;

    /**
     * @brief Unbinds the current VertexBuffer
     * 
     */
    void unbind() const;
};