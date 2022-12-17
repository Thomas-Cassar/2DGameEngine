module;
#include "graphics/Common.hpp"
export module Graphics;

export import :IndexBuffer;
export import :Shader;
export import :VertexBufferLayout;
export import :VertexArray;
export import :VertexBuffer;
export import :Texture2D;

namespace Graphics
{
export void drawBoundBuffers(unsigned int indexBufCount)
{
    glCheck(glDrawElements(GL_TRIANGLES, indexBufCount, GL_UNSIGNED_INT, nullptr));
}
} // namespace Graphics
