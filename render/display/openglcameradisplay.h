#ifndef SAHARA_OPENGL_CAMERADISPLAY_H
#define SAHARA_OPENGL_CAMERADISPLAY_H

#include "opengldisplay.h"

namespace Sahara {

    class OpenGLCameraDisplay : public OpenGLDisplay
    {
        public:
            OpenGLCameraDisplay();

        private:
            void initBuffers();
    };

}

#endif // SAHARA_OPENGL_CAMERADISPLAY_H
