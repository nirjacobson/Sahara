#ifndef SAHARA_VULKAN_VERTEXBUFFER_H
#define SAHARA_VULKAN_VERTEXBUFFER_H

#include <QVulkanDeviceFunctions>
#include <QVulkanWindow>
#include <QtDebug>
#include <QMap>

#include "vulkanutil.h"

namespace Sahara
{

    class VulkanVertexBuffer
    {
        public:
            VulkanVertexBuffer(QVulkanWindow *window);
            ~VulkanVertexBuffer();

            VkBuffer buffer() const;

            void write(const float * const data, const uint32_t size, int stride);

            uint32_t count() const;

        private:
            QVulkanWindow* _window;
            VkBuffer _buffer;
            VkDeviceMemory _memory;

            uint32_t _size;
            int _stride;
            bool _haveBuffer;
    };

    typedef QMap<QString, VulkanVertexBuffer*> VulkanVertexBufferDict;
}

#endif // SAHARA_VULKAN_VERTEXBUFFER_H
