export module Graphics;

import :IndexBuffer;
import :Shader;
import :VertexBufferLayout;
import :VertexArray;
import :VertexBuffer;
import :Mesh;
import :MeshImpl;
import :ResourceManager;
export import :Texture2D;
export import :Model;
export import :ResourceManagerImpl;

import "graphics/Common.hpp";

namespace Graphics
{
export void drawBoundBuffers(unsigned int indexBufCount)
{
    glCheck(glDrawElements(GL_TRIANGLES, indexBufCount, GL_UNSIGNED_INT, nullptr));
}
} // namespace Graphics
