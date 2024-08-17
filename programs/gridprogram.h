#ifndef SAHARA_GRIDPROGRAM_H
#define SAHARA_GRIDPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"
#include "../render/grid/openglgrid.h"

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

            void setGrid(OpenGLGrid& grid);
            void clearGrid(OpenGLGrid& grid);
            void setAxis(OpenGLAxis& axis);
            void clearAxis(OpenGLAxis& axis);
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
