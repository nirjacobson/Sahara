#ifndef SAHARA_VULKAN_POINTLIGHTDISPLAY_H
#define SAHARA_VULKAN_POINTLIGHTDISPLAY_H

#include <QList>

#include "vulkandisplay.h"

namespace Sahara {

class VulkanPointLightDisplay : public VulkanDisplay
    {
        public:
            VulkanPointLightDisplay(QVulkanWindow *window);

            int count() const;
        private:
            QVulkanWindow* _vulkanWindow;
            void initBuffers();
    };

}

#endif // SAHARA_VULKAN_POINTLIGHTDISPLAY_H
