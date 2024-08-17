#ifndef SAHARA_OPENGL_POINTLIGHTDISPLAY_H
#define SAHARA_OPENGL_POINTLIGHTDISPLAY_H

#include <QList>

#include "opengldisplay.h"

namespace Sahara {

class OpenGLPointLightDisplay : public OpenGLDisplay
    {
        public:
            OpenGLPointLightDisplay();

        private:
            void initBuffers();
    };

}

#endif // SAHARA_OPENGL_POINTLIGHTDISPLAY_H
