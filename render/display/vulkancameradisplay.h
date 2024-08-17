#ifndef SAHARA_VULKAN_CAMERADISPLAY_H
#define SAHARA_VULKAN_CAMERADISPLAY_H

#include "vulkandisplay.h"

namespace Sahara {

    class VulkanCameraDisplay : public VulkanDisplay
    {
        public:
            VulkanCameraDisplay(QVulkanWindow* window);

        private:
            QVulkanWindow* _vulkanWindow;

            void initBuffers();

            // WithVertexBuffers interface
        public:
            int count() const;
    };

}

#endif // SAHARA_CAMERADISPLAY_H
