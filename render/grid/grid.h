#ifndef SAHARA_GRID_H
#define SAHARA_GRID_H

#include <QVector3D>

#include "../withvertexbuffers.h"
#include "axis.h"

namespace Sahara {

    class Grid : public WithVertexBuffers
    {
        public:
            Grid(QVulkanWindow *window, const int length);

            int length() const;

            Axis& xAxis();
            Axis& yAxis();
            Axis& zAxis();

            int count() const;

        private:
            QVulkanWindow* _vulkanWindow;

            int _length;
            Axis _xAxis;
            Axis _yAxis;
            Axis _zAxis;

            void initPositionBuffer();
            void initColorBuffer();
    };

}

#endif // SAHARA_GRID_H
