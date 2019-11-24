#include "vertexbuffer.h"


Sahara::VertexBuffer::VertexBuffer(const GLenum type)
    : _type(type)
{
    _buffer.create();
}

Sahara::VertexBuffer::~VertexBuffer()
{

}

GLenum Sahara::VertexBuffer::type() const
{
    return _type;
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
    int componentSize;
    switch (_type) {
        default:
        case GL_FLOAT:
            componentSize = sizeof(GLfloat);
            break;
        case GL_INT:
            componentSize = sizeof(GLint);
            break;
    }

    return _size / (_stride * componentSize);
}

void Sahara::VertexBuffer::bind()
{
    _buffer.bind();
}

void Sahara::VertexBuffer::release()
{
    _buffer.release();
}

void Sahara::VertexBuffer::write(const char* const data, const int size)
{
    _buffer.bind();
    _buffer.allocate(size);
    _buffer.write(0, data, size);
    _buffer.release();

    assert(glGetError() == GL_NO_ERROR);

    _size = size;
}

void Sahara::VertexBuffer::setStride(const int stride)
{
    _stride = stride;
}
