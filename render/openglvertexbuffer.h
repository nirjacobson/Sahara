#ifndef SAHARA_OPENGL_VERTEXBUFFER_H
#define SAHARA_OPENGL_VERTEXBUFFER_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QtDebug>
#include <QMap>

namespace Sahara
{

    class OpenGLVertexBuffer
    {
        public:
            OpenGLVertexBuffer();
            ~OpenGLVertexBuffer();

            int stride() const;
            int size() const;
            int count() const;

            void bind();
            void release();
            void write(const GLfloat* const data, const int size);
            void setStride(const int stride);

        private:
            QOpenGLBuffer _buffer;
            int _stride;
            int _size;
    };

    typedef QMap<QString, OpenGLVertexBuffer> OpenGLVertexBufferDict;
}

#endif // SAHARA_OPENGL_VERTEXBUFFER_H
