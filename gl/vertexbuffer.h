#ifndef SAHARA_VERTEXBUFFER_H
#define SAHARA_VERTEXBUFFER_H

#include <QOpenGLBuffer>

namespace Sahara
{

    class VertexBuffer
    {
        public:
            VertexBuffer(const GLenum type);
            ~VertexBuffer();

            GLenum type() const;

            int stride() const;
            int size() const;
            int count() const;

            void bind();
            void release();
            void write(const char* const data, const int size);
            void setStride(const int stride);

        private:
            QOpenGLBuffer _buffer;
            GLenum _type;
            int _stride;
            int _size;
    };

}

#endif // SAHARA_VERTEXBUFFER_H
