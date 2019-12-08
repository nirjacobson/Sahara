#ifndef SAHARA_AXIS_H
#define SAHARA_AXIS_H

#include <QVector3D>
#include <QMatrix4x4>

#include "../withvertexbuffers.h"

namespace Sahara {

    class Axis : public WithVertexBuffers
    {
        public:
            Axis(const char axis, const int length);

        private:
            char _axis;
            int _length;

            void initPositionBuffer();
            void initColorBuffer();
    };

}

#endif // SAHARA_AXIS_H
