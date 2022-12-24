export module Graphics;

import :IndexBuffer;
import :Shader;
import :VertexBufferLayout;
import :VertexArray;
import :VertexBuffer;
import :Mesh;
export import :Texture2D;
export import :Model;
export import :Resource;
export import :ResourceManager;

import "graphics/Common.hpp";


namespace Graphics
{
export void drawBoundBuffers(unsigned int indexBufCount);
} // namespace Graphics
