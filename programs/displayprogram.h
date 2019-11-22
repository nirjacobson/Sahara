#ifndef SAHARA_DISPLAYPROGRAM_H
#define SAHARA_DISPLAYPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"

namespace Sahara
{

    class DisplayProgram : public Program
    {
        public:
            struct VertexRenderUniforms {
                GLint modelView;
                GLint inverseCamera;
                GLint projection;
            };

            struct FragmentRenderUniforms {
                GLint cameraPosition;
                GLint focus;
            };

            DisplayProgram();
            ~DisplayProgram();

            void bind() override;
            void release() override;

            void setPosition(const int stride, const void* offset);
            void setNormal(const int stride, const void* offset);

            void setModelView(const QMatrix4x4& modelView);
            void setInverseCamera(const QMatrix4x4& inverseCamera);
            void setProjection(const QMatrix4x4& projection);
            void setCameraPosition(const QVector3D& cameraPosition);
            void setFocus(const bool focus);

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShaderProgram* _program;

            GLint _position;
            GLint _normal;

            VertexRenderUniforms _vertexRender;
            FragmentRenderUniforms _fragmentRender;
    };

}

#endif // SAHARA_DISPLAYPROGRAM_H
