#ifndef SAHARA_OPENGL_AXIS_H
#define SAHARA_OPENGL_AXIS_H

#include <QVector3D>
#include <QMatrix4x4>

#include "../openglwithvertexbuffers.h"

namespace Sahara {

    class OpenGLAxis : public OpenGLWithVertexBuffers
    {
        public:
            OpenGLAxis(const char axis, const int length);

        private:
            char _axis;
            int _length;

            void initPositionBuffer();
            void initColorBuffer();
    };

}

#endif // SAHARA_OPENGL_AXIS_H
