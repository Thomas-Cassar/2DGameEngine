export module Graphics:IndexBuffer;

import "graphics/Common.hpp";

namespace Graphics
{
export class IndexBuffer {
private:
    /** ID given to this index buffer by OpenGL*/
    unsigned int ibID;
    /** The count of indicies in the buffer*/
    unsigned int count;

public:
    /**
     * @brief Construct a new Index Buffer object
     *
     * @param data Array of indicies
     * @param count Count of indicies
     */
    IndexBuffer(const unsigned int* data, unsigned int count) : count(count)
    {
        glCheck(glGenBuffers(1, &ibID));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, ibID));
        glCheck(glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    }

    IndexBuffer(IndexBuffer const&) = delete;

    IndexBuffer& operator=(IndexBuffer const&) = delete;

    /**
     * @brief Destroy the Index Buffer object
     *
     */
    ~IndexBuffer() { glCheck(glDeleteBuffers(1, &ibID)); }

    /**
     * @brief Bind this as the current index buffer
     *
     */
    void bind() const { glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibID)); }

    /**
     * @brief Unbind the current index buffer
     *
     */
    void unbind() const { glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

    /**
     * @brief Get the Count object
     *
     * @return unsigned int
     */
    unsigned int getCount() const { return count; }
};
} // namespace Graphics