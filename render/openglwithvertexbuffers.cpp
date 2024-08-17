#include "openglwithvertexbuffers.h"


Sahara::OpenGLWithVertexBuffers::OpenGLWithVertexBuffers()
{

}

const Sahara::OpenGLVertexBufferDict &Sahara::OpenGLWithVertexBuffers::vertexBuffers() const
{
    return _vertexBuffers;
}

Sahara::OpenGLVertexBufferDict &Sahara::OpenGLWithVertexBuffers::vertexBuffers()
{
    return _vertexBuffers;
}

void Sahara::OpenGLWithVertexBuffers::addVertexBuffer(const QString &name, const OpenGLVertexBuffer& vertexBuffer)
{
    _vertexBuffers.insert(name, vertexBuffer);
}
