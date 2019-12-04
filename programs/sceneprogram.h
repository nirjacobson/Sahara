#ifndef SAHARA_SCENEPROGRAM_H
#define SAHARA_SCENEPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"
#include "scene/asset/bone.h"
#include "scene/asset/pointlight.h"
#include "scene/asset/material.h"
#include "scene/asset/surface.h"
#include "common/transform.h"

namespace Sahara
{

    class SceneProgram : public Program
    {
        public:
            static constexpr int MAX_BONES = 66;
            static constexpr int MAX_POINT_LIGHTS = 6;

            struct RenderUniforms {
                GLint modelView;
                GLint inverseCamera;
                GLint projection;
                GLint boned;
            };

            struct PointLightUniforms {
                GLint position;
                GLint color;
                GLint constantAttenuation;
                GLint linearAttenuation;
                GLint quadraticAttenuation;
            };

            struct LightingUniforms {
                PointLightUniforms pointLights[MAX_POINT_LIGHTS];
                GLint pointLightCount;
            };

            struct MaterialUniforms {
                GLint emission;
                GLint ambient;
                GLint diffuse;
                GLint specular;
                GLint shininess;
                GLint textured;
            };


            SceneProgram();
            ~SceneProgram();

            void bind() override;
            void release() override;

            void setSurface(Surface& surface);
            void clearSurface(Surface& surface);

            void setModelView(const QMatrix4x4& modelView);
            void setInverseCamera(const QMatrix4x4& inverseCamera);
            void setProjection(const QMatrix4x4& projection);
            void setBoned(const bool boned);
            void setFocus(const bool focus);
            void setBoneTransforms(const QList<Transform>& transforms);

            void addPointLight(const PointLight& pointLight, const QVector3D& position);
            void clearPointLights();
            void setMaterial(const Material& material);
            void setCameraPosition(const QVector3D& position);

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShaderProgram* _program;

            RenderUniforms _render;

            GLint _boneRotations;
            GLint _boneTranslations;

            LightingUniforms _lighting;
            MaterialUniforms _material;

            GLint _focus;
            GLint _cameraPosition;
            GLint _sampler;

            int _pointLights;
    };

}

#endif // SAHARA_SCENEPROGRAM_H
