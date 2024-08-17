#ifndef SAHARA_OPENGL_GRID_H
#define SAHARA_OPENGL_GRID_H

#include <QVector3D>

#include "../openglwithvertexbuffers.h"
#include "openglaxis.h"

namespace Sahara {

    class OpenGLGrid : public OpenGLWithVertexBuffers
    {
        public:
            OpenGLGrid(const int length);

            int length() const;

            OpenGLAxis& xAxis();
            OpenGLAxis& yAxis();
            OpenGLAxis& zAxis();

        private:
            int _length;
            OpenGLAxis _xAxis;
            OpenGLAxis _yAxis;
            OpenGLAxis _zAxis;

            void initPositionBuffer();
            void initColorBuffer();
    };

}

#endif // SAHARA_OPENGL_GRID_H
