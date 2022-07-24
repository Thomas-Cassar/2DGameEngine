#include "renderable/BaseMesh.hpp"

BaseMesh::BaseMesh()
{
}

BaseMesh::BaseMesh(
    VertexBuffer *vb, VertexBufferLayout *vbl, VertexArray *va, IndexBuffer *ib, Shader *shad)
    : vb(vb),
      vbl(vbl),
      va(va),
      ib(ib),
      shad(shad)
{
}

BaseMesh::~BaseMesh()
{
    delete vb;
    delete vbl;
    delete va;
    delete ib;
    delete shad;
}

void BaseMesh::render(glm::mat4 projectionViewMatrix)
{
    if (vb == nullptr || vbl == nullptr || va == nullptr || ib == nullptr || shad == nullptr)
        return;
    shad->bind();
    glm::mat4 mvp = projectionViewMatrix * getModelMatrix();
    shad->SetUniformMat4f("MVP", mvp);
    va->bind();
    ib->bind();
    glCheck(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));
}
