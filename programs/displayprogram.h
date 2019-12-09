#ifndef SAHARA_DISPLAYPROGRAM_H
#define SAHARA_DISPLAYPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"
#include "../render/display/display.h"

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

            void setDisplay(Display& display);
            void clearDisplay(Display& display);
            void setModelView(const QMatrix4x4& modelView);
            void setInverseCamera(const QMatrix4x4& inverseCamera);
            void setProjection(const QMatrix4x4& projection);
            void setCameraPosition(const QVector3D& cameraPosition);
            void setFocus(const bool focus);

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;

            GLint _position;
            GLint _normal;

            VertexRenderUniforms _vertexRender;
            FragmentRenderUniforms _fragmentRender;
    };

}

#endif // SAHARA_DISPLAYPROGRAM_H
