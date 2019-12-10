#include "surface.h"

Sahara::Surface::Surface(const QString& material)
    : _material(material)
{

}

void Sahara::Surface::addVertexBuffer(const QString& name, const GLfloat* const data, const int size, const int stride)
{
    VertexBuffer buffer;
    buffer.setStride(stride);
    buffer.write(data, size);

    WithVertexBuffers::addVertexBuffer(name, buffer);
}

QString Sahara::Surface::material() const
{
    return _material;
}

