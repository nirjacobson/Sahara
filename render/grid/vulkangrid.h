#ifndef SAHARA_VULKAN_GRID_H
#define SAHARA_VULKAN_GRID_H

#include <QVector3D>

#include "../vulkanwithvertexbuffers.h"
#include "vulkanaxis.h"

namespace Sahara {

    class VulkanGrid : public VulkanWithVertexBuffers
    {
        public:
            VulkanGrid(QVulkanWindow *window, const int length);

            int length() const;

            VulkanAxis& xAxis();
            VulkanAxis& yAxis();
            VulkanAxis& zAxis();

            int count() const;

        private:
            QVulkanWindow* _vulkanWindow;

            int _length;
            VulkanAxis _xAxis;
            VulkanAxis _yAxis;
            VulkanAxis _zAxis;

            void initPositionBuffer();
            void initColorBuffer();
    };

}

#endif // SAHARA_VULKAN_GRID_H
