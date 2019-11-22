#ifndef SAHARA_GRIDPROGRAM_H
#define SAHARA_GRIDPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"

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

            void setPosition(const int stride, const void* offset);

            void setInverseCamera(const QMatrix4x4& inverseCamera);
            void setProjection(const QMatrix4x4& projection);

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShaderProgram* _program;

            GLint _position;

            RenderUniforms _render;
    };

}

#endif // SAHARA_GRIDPROGRAM_H
