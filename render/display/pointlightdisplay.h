#ifndef SAHARA_POINTLIGHTDISPLAY_H
#define SAHARA_POINTLIGHTDISPLAY_H

#include <QList>

#include "display.h"

namespace Sahara {

class PointLightDisplay : public Display
    {
        public:
            PointLightDisplay(QVulkanWindow *window);

            int count() const;
        private:
            QVulkanWindow* _vulkanWindow;
            void initBuffers();
    };

}

#endif // SAHARA_POINTLIGHTDISPLAY_H
