#include "renderable/Plane2D.hpp"

const unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3};

Plane2D::Plane2D(float width, float height, glm::vec3 color)
    : width(width),
      height(height)
{
    const float verArray[] =
        {
            // Translation                                   Colors
            -1.0f * width / 2.0f, -1.0f * height / 2.0f, 0.0f, color.x, color.y, color.z,
            1.0f * width / 2.0f, -1.0f * height / 2.0f, 0.0f, color.x, color.y, color.z,
            1.0f * width / 2.0f, 1.0f * height / 2.0f, 0.0f, color.x, color.y, color.z,
            -1.0f * width / 2.0f, 1.0f * height / 2.0f, 0.0f, color.x, color.y, color.z};
    vb = new VertexBuffer(verArray, sizeof(verArray));

    vbl = new VertexBufferLayout;
    vbl->push(GL_FLOAT, 3, false);
    vbl->push(GL_FLOAT, 3, false);

    va = new VertexArray;
    va->addBuffer(*vb, *vbl);

    ib = new IndexBuffer(indicies, sizeof(indicies) / sizeof(indicies[0]));

    shad = new Shader("shaders/Plane2DColored.vert", "shaders/Plane2DColored.frag");
}

Plane2D::Plane2D(float width, float height, std::string const &texture)
    : width(width),
      height(height)
{
    const float verArray[] =
        {
            // Translation                                   Texture Coords
            -1.0f * width / 2.0f, -1.0f * height / 2.0f, 0.0f, 0.0f, 0.0f,
            1.0f * width / 2.0f, -1.0f * height / 2.0f, 0.0f, 1.0f, 0.0f,
            1.0f * width / 2.0f, 1.0f * height / 2.0f, 0.0f, 1.0f, 1.0f,
            -1.0f * width / 2.0f, 1.0f * height / 2.0f, 0.0f, 0.0f, 1.0f};
    vb = new VertexBuffer(verArray, sizeof(verArray));

    vbl = new VertexBufferLayout;
    vbl->push(GL_FLOAT, 3, false);
    vbl->push(GL_FLOAT, 2, false);

    va = new VertexArray;
    va->addBuffer(*vb, *vbl);

    ib = new IndexBuffer(indicies, sizeof(indicies) / sizeof(indicies[0]));

    shad = new Shader("shaders/Plane2DTextured.vert", "shaders/Plane2DTextured.frag");
    shad->bind();
    shad->setUniform1i("texture1", 0);
    tex = new Texture2D(texture);
}

Plane2D::~Plane2D()
{
    delete tex;
}

void Plane2D::render(glm::mat4 projectionViewMatrix)
{
    if (tex != nullptr)
        tex->bind();
    BaseMesh::render(projectionViewMatrix);
}
