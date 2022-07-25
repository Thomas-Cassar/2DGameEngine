#pragma once

class IndexBuffer {
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
    IndexBuffer(const unsigned int *data, unsigned int count);
    /**
     * @brief Destroy the Index Buffer object
     *
     */
    ~IndexBuffer();

    /**
     * @brief Bind this as the current index buffer
     *
     */
    void bind() const;
    /**
     * @brief Unbind the current index buffer
     *
     */
    void unbind() const;

    /**
     * @brief Get the Count object
     *
     * @return unsigned int
     */
    unsigned int getCount() const;
};