#ifndef SAHARA_GRIDPROGRAM_H
#define SAHARA_GRIDPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"
#include "../render/grid/grid.h"

namespace Sahara
{

    class GridProgram : public Program
    {
        public:
            struct RenderUniforms {
                GLint inverseCamera;
                GLint projection;
            };

            GridProgram();
            ~GridProgram();

            void bind() override;
            void release() override;

            void setGrid(Grid& grid);
            void clearGrid(Grid& grid);
            void setAxis(Axis& axis);
            void clearAxis(Axis& axis);
            void setInverseCamera(const QMatrix4x4& inverseCamera);
            void setProjection(const QMatrix4x4& projection);

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;

            GLint _position;

            RenderUniforms _render;
    };

}

#endif // SAHARA_GRIDPROGRAM_H
