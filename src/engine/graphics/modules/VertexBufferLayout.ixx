module;
#include <vector>
export module Graphics:VertexBufferLayout;

namespace Graphics
{
/** Struct used to represent an element in a VertexBuffer*/
export struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    /**
     * @brief Get the Size in Bytes Of OpenGL type
     *
     * @param type Enum representing the type
     * @return unsigned int Size in bytes
     */
    static unsigned int getSizeOfGLType(unsigned int type);
};

export class VertexBufferLayout {

public:
    /**
     * @brief Construct a new Vertex Buffer Layout object
     *
     */
    VertexBufferLayout();

    /**
     * @brief Push an attribute into the layout
     *
     * @param type The OpenGL type to push (GL_FLOAT, GL_UNSIGNED_INT etc.)
     * @param count The number of that type in this attribute
     * @param normalize If the data type is normalized
     */
    void push(unsigned int type, unsigned int count, bool normalize);

    /**
     * @brief Get the Elements object
     *
     * @return const std::vector<VertexBufferElement>
     */
    const std::vector<VertexBufferElement> getElements() const;

    /**
     * @brief Get the Stride object
     *
     * @return unsigned int
     */
    unsigned int getStride() const;

private:
    /** Vector of VertexBufferElements representing the order and layout of elements in the buffer */
    std::vector<VertexBufferElement> elements;
    /** Size in bytes between attributes of each vertex in the buffer*/
    unsigned int stride;
};
} // namespace Graphics