#include "withvertexbuffers.h"


Sahara::WithVertexBuffers::WithVertexBuffers()
{

}

const Sahara::VertexBufferDict &Sahara::WithVertexBuffers::vertexBuffers() const
{
    return _vertexBuffers;
}

Sahara::VertexBufferDict &Sahara::WithVertexBuffers::vertexBuffers()
{
    return _vertexBuffers;
}

void Sahara::WithVertexBuffers::addVertexBuffer(const QString &name, const VertexBuffer& vertexBuffer)
{
    _vertexBuffers.insert(name, vertexBuffer);
}
