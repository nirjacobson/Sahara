#include "surface.h"

Sahara::Surface::Surface(const QString& material)
    : _material(material)
{

}

void Sahara::Surface::addVertexBuffer(const QString& name, const GLenum type, const char* const data, const int size, const int stride)
{
    VertexBuffer buffer(type);
    buffer.setStride(stride);
    buffer.write(data, size);

    _vertexBuffers.insert(name, buffer);
}

Sahara::VertexBufferDict& Sahara::Surface::vertexBuffers()
{
    return _vertexBuffers;
}

const Sahara::VertexBufferDict& Sahara::Surface::vertexBuffers() const
{
    return _vertexBuffers;
}

QString Sahara::Surface::material() const
{
    return _material;
}

