#ifndef SAHARA_SCENEPROGRAM_H
#define SAHARA_SCENEPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"
#include "scene/asset/light/ambientlight.h"
#include "scene/asset/light/pointlight.h"
#include "scene/asset/material.h"
#include "scene/asset/mesh/surface.h"
#include "common/transform.h"

namespace Sahara
{

    class SceneProgram : public Program
    {
        public:
            static constexpr int MAX_JOINTS = 75;
            static constexpr int MAX_POINT_LIGHTS = 6;

            struct RenderUniforms {
                GLint modelView;
                GLint inverseCamera;
                GLint projection;
            };

            struct JointUniforms {
                GLint rotation;
                GLint translation;
            };

            struct ArmatureUniforms {
                JointUniforms joints[MAX_JOINTS];
                GLint present;
            };

            struct AmbientLightUniforms {
                GLint color;
                GLint strength;
            };

            struct PointLightUniforms {
                GLint position;
                GLint color;
                GLint constantAttenuation;
                GLint linearAttenuation;
                GLint quadraticAttenuation;
            };

            struct LightingUniforms {
                AmbientLightUniforms ambientLight;
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
            ~SceneProgram() override;

            void bind() override;
            void release() override;

            void setSurface(Surface& surface);
            void clearSurface(Surface& surface);

            void setModelView(const QMatrix4x4& modelView);
            void setInverseCamera(const QMatrix4x4& inverseCamera);
            void setProjection(const QMatrix4x4& projection);
            void setArticulated(const bool articulated);
            void setFocus(const bool focus);
            void setJointTransforms(const QList<Transform>& transforms);

            void setAmbientLight(const AmbientLight& ambientLight);
            void addPointLight(const PointLight& pointLight, const QVector3D& position);
            void clearPointLights();
            void setMaterial(const Material& material);
            void setCameraPosition(const QVector3D& position);

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;

            RenderUniforms _render;

            ArmatureUniforms _armature;

            LightingUniforms _lighting;
            MaterialUniforms _material;

            GLint _focus;
            GLint _cameraPosition;
            GLint _sampler;

            int _pointLights;
    };

}

#endif // SAHARA_SCENEPROGRAM_H
