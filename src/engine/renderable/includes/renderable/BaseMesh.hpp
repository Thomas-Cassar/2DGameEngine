#pragma once
#include "renderable/Renderable.hpp"

#include "graphics/VertexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/Shader.hpp"


#include <string>

class BaseMesh : public Renderable
{
    protected:
    VertexBuffer* vb;
	VertexBufferLayout* vbl;
    VertexArray* va;
    IndexBuffer* ib;
    Shader* shad;

    public:
    BaseMesh(bool doesTick),
    BaseMesh(bool doesTick,
        VertexBuffer* vb, VertexBufferLayout* vbl, VertexArray* va,IndexBuffer* ib,Shader* shad);
    ~BaseMesh();
    void render(glm::mat4 projectionViewMatrix);
};