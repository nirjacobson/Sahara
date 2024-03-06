#ifndef SAHARA_VERTEXBUFFER_H
#define SAHARA_VERTEXBUFFER_H

#include <QOpenGLBuffer>
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
            VertexBuffer(); // in order to support containers
            VertexBuffer(QVulkanWindow *window);
            ~VertexBuffer();

            VkBuffer buffer() const;

            void write(const float * const data, const uint32_t size);

        private:
            QVulkanWindow* _window;
            QVulkanDeviceFunctions* _deviceFunctions;
            VkBuffer _buffer;
            VkDeviceMemory _memory;

            bool _haveBuffer;
    };

    typedef QMap<QString, VertexBuffer> VertexBufferDict;
}

#endif // SAHARA_VERTEXBUFFER_H
