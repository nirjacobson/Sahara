#ifndef SAHARA_CAMERADISPLAY_H
#define SAHARA_CAMERADISPLAY_H

#include "display.h"

namespace Sahara {

    class CameraDisplay : public Display
    {
        public:
            CameraDisplay();

        private:
            void initBuffers();
    };

}

#endif // SAHARA_CAMERADISPLAY_H
