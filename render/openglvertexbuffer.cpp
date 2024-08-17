#include "openglvertexbuffer.h"


Sahara::OpenGLVertexBuffer::OpenGLVertexBuffer()
{
    _buffer.create();
}

Sahara::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{

}

int Sahara::OpenGLVertexBuffer::stride() const
{
    return _stride;
}

int Sahara::OpenGLVertexBuffer::size() const
{
    return _size;
}

int Sahara::OpenGLVertexBuffer::count() const
{
    return _size / _stride;
}

void Sahara::OpenGLVertexBuffer::bind()
{
    _buffer.bind();
}

void Sahara::OpenGLVertexBuffer::release()
{
    _buffer.release();
}

void Sahara::OpenGLVertexBuffer::write(const GLfloat* const data, const int size)
{
    int sizeBytes = static_cast<int>(sizeof(GLfloat)) * size;
    _buffer.bind();
    _buffer.allocate(sizeBytes);
    _buffer.write(0, data, sizeBytes);
    _buffer.release();

    _size = size;
}

void Sahara::OpenGLVertexBuffer::setStride(const int stride)
{
    _stride = stride;
}
