module Graphics;
import "graphics/Common.hpp";

namespace Graphics
{
void drawBoundBuffers(unsigned int indexBufCount)
{
    glCheck(glDrawElements(GL_TRIANGLES, indexBufCount, GL_UNSIGNED_INT, nullptr));
}
} // namespace Graphics
