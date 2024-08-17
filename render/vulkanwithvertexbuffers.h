#ifndef SAHARA_VULKAN_WITHVERTEXBUFFERS_H
#define SAHARA_VULKAN_WITHVERTEXBUFFERS_H

#include "vulkanvertexbuffer.h"
#include "pipelines/pipeline.h"

namespace Sahara {
    class VulkanWithVertexBuffers
    {
        public:
            VulkanWithVertexBuffers();
            ~VulkanWithVertexBuffers();

            const VulkanVertexBufferDict& vertexBuffers() const;
            VulkanVertexBufferDict& vertexBuffers();

            QList<VkBuffer> buffersByBinding(const Pipeline& pipeline);

        protected:
            void addVertexBuffer(const QString& name, VulkanVertexBuffer *vertexBuffer);

        private:
            VulkanVertexBufferDict _vertexBuffers;
    };
}

#endif // SAHARA_WITHVERTEXBUFFERS_H
