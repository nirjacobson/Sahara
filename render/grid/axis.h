#ifndef SAHARA_AXIS_H
#define SAHARA_AXIS_H

#include <QVector3D>
#include <QMatrix4x4>

#include "../withvertexbuffers.h"

namespace Sahara {

    class Axis : public WithVertexBuffers
    {
        public:
            Axis(QVulkanWindow *window, const char axis, const int length);

        private:
            QVulkanWindow* _vulkanWindow;
            char _axis;
            int _length;

            void initPositionBuffer();
            void initColorBuffer();

            // WithVertexBuffers interface
        public:
            int count() const;
    };

}

#endif // SAHARA_AXIS_H
