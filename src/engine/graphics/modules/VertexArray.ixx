export module Graphics:VertexArray;
export import :VertexBufferLayout;
export import :VertexBuffer;

namespace Graphics
{

export class VertexArray {
public:
    /**
     * @brief Construct a new Vertex Array object
     * @post vaID is given an ID by OpenGL
     */
    VertexArray();

    VertexArray(VertexArray const&) = delete;

    VertexArray& operator=(VertexArray const&) = delete;

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
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

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

private:
    /** ID given to this VA by OpenGL*/
    unsigned int vaID;
};
} // namespace Graphics