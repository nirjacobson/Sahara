#ifndef SAHARA_POINTLIGHTDISPLAY_H
#define SAHARA_POINTLIGHTDISPLAY_H

#include <QList>

#include "display.h"

namespace Sahara {

class PointLightDisplay : public Display
    {
        public:
            PointLightDisplay();

        private:
            void initBuffers();
    };

}

#endif // SAHARA_POINTLIGHTDISPLAY_H
