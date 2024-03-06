#ifndef SAHARA_CAMERADISPLAY_H
#define SAHARA_CAMERADISPLAY_H

#include "display.h"

namespace Sahara {

    class CameraDisplay : public Display
    {
        public:
            CameraDisplay(QVulkanWindow* window);

        private:
            QVulkanWindow* _vulkanWindow;

            void initBuffers();

            // WithVertexBuffers interface
        public:
            int count() const;
    };

}

#endif // SAHARA_CAMERADISPLAY_H
