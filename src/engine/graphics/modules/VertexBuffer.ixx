export module Graphics:VertexBuffer;

namespace Graphics
{

export class VertexBuffer {
public:
    /**
     * @brief Construct a new Vertex Buffer object
     *
     * @param data Array of vertices
     * @param size Size in bytes of the vertex buffer
     * @post vbID is set to an ID given by OpenGL
     */
    VertexBuffer(const void* data, unsigned int size);

    VertexBuffer(VertexBuffer const&) = delete;

    VertexBuffer& operator=(VertexBuffer const&) = delete;

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

private:
    /** ID given to this vertex buffer by OpenGL*/
    unsigned int vbID;
};
} // namespace Graphics