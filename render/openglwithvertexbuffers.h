#ifndef SAHARA_OPENGL_WITHVERTEXBUFFERS_H
#define SAHARA_OPENGL_WITHVERTEXBUFFERS_H

#include "openglvertexbuffer.h"

namespace Sahara {
    class OpenGLWithVertexBuffers
    {
        public:
            OpenGLWithVertexBuffers();

            OpenGLVertexBufferDict& vertexBuffers();
            const OpenGLVertexBufferDict& vertexBuffers() const;

        protected:
            void addVertexBuffer(const QString& name, const OpenGLVertexBuffer& vertexBuffer);

        private:
            OpenGLVertexBufferDict _vertexBuffers;
    };
}

#endif // SAHARA_OPENGL_WITHVERTEXBUFFERS_H
