#ifndef SAHARA_VERTEXBUFFER_H
#define SAHARA_VERTEXBUFFER_H

#include <QVulkanDeviceFunctions>
#include <QVulkanWindow>
#include <QtDebug>
#include <QMap>

#include "vulkanutil.h"

namespace Sahara
{

    class VertexBuffer
    {
        public:
            VertexBuffer(QVulkanWindow *window);
            ~VertexBuffer();

            VkBuffer buffer() const;

            void write(const float * const data, const uint32_t size, int stride);

            uint32_t count() const;

        private:
            QVulkanWindow* _window;
            QVulkanDeviceFunctions* _deviceFunctions;
            VkBuffer _buffer;
            VkDeviceMemory _memory;

            uint32_t _size;
            int _stride;
            bool _haveBuffer;
    };

    typedef QMap<QString, VertexBuffer*> VertexBufferDict;
}

#endif // SAHARA_VERTEXBUFFER_H
