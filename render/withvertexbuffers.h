#ifndef SAHARA_WITHVERTEXBUFFERS_H
#define SAHARA_WITHVERTEXBUFFERS_H

#include "vertexbuffer.h"
#include "pipelines/pipeline.h"

namespace Sahara {
    class WithVertexBuffers
    {
        public:
            WithVertexBuffers();
            ~WithVertexBuffers();

            const VertexBufferDict& vertexBuffers() const;
            VertexBufferDict& vertexBuffers();

            QList<VkBuffer> buffersByBinding(const Pipeline& pipeline);

        protected:
            void addVertexBuffer(const QString& name, VertexBuffer *vertexBuffer);

        private:
            VertexBufferDict _vertexBuffers;
    };
}

#endif // SAHARA_WITHVERTEXBUFFERS_H
