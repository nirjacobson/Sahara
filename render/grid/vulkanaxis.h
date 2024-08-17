#ifndef SAHARA_VULKAN_AXIS_H
#define SAHARA_VULKAN_AXIS_H

#include <QVector3D>
#include <QMatrix4x4>

#include "../vulkanwithvertexbuffers.h"

namespace Sahara {

    class VulkanAxis : public VulkanWithVertexBuffers
    {
        public:
            VulkanAxis(QVulkanWindow *window, const char axis, const int length);

        private:
            QVulkanWindow* _vulkanWindow;
            char _axis;
            int _length;

            void initPositionBuffer();
            void initColorBuffer();

            // WithVertexBuffers interface
        public:
            int count() const;
    };

}

#endif // SAHARA_VULKAN_AXIS_H
