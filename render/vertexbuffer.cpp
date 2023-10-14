#include "vertexbuffer.h"


Sahara::VertexBuffer::VertexBuffer()
{
    _buffer.create();
}

Sahara::VertexBuffer::~VertexBuffer()
{

}

int Sahara::VertexBuffer::stride() const
{
    return _stride;
}

int Sahara::VertexBuffer::size() const
{
    return _size;
}

int Sahara::VertexBuffer::count() const
{
    return _size / _stride;
}

void Sahara::VertexBuffer::bind()
{
    _buffer.bind();
}

void Sahara::VertexBuffer::release()
{
    _buffer.release();
}

void Sahara::VertexBuffer::write(const GLfloat* const data, const int size)
{
    int sizeBytes = static_cast<int>(sizeof(GLfloat)) * size;
    _buffer.bind();
    _buffer.allocate(sizeBytes);
    _buffer.write(0, data, sizeBytes);
    _buffer.release();

    _size = size;
}

void Sahara::VertexBuffer::setStride(const int stride)
{
    _stride = stride;
}
