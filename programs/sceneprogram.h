#ifndef SAHARA_SCENEPROGRAM_H
#define SAHARA_SCENEPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "program.h"
#include "scene/asset/bone.h"
#include "scene/asset/pointlight.h"
#include "scene/asset/material.h"

namespace Sahara
{

    class SceneProgram : public Program
    {
        public:
            static constexpr int MAX_BONES = 74;
            static constexpr int MAX_POINT_LIGHTS = 12;

            struct RenderUniforms {
                GLint modelView;
                GLint inverseCamera;
                GLint projection;
                GLint boned;
                GLint focus;
            };

            struct BoneUniforms {
                GLint rotation;
                GLint translation;
            };

            struct ArmatureUniforms {
                BoneUniforms bones[MAX_BONES];
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

            void setPosition(const int stride, const void* offset);
            void setNormal(const int stride, const void* offset);
            void setTexcoord(const int stride, const void* offset);
            void setBones1(const int stride, const void* offset);
            void setBones2(const int stride, const void* offset);
            void setWeights1(const int stride, const void* offset);
            void setWeights2(const int stride, const void* offset);

            void setModelView(const QMatrix4x4& modelView);
            void setInverseCamera(const QMatrix4x4& inverseCamera);
            void setProjection(const QMatrix4x4& projection);
            void setBoned(const bool boned);
            void setFocus(const bool focus);
            void setBone(const int index, const Bone& bone);

            void addPointLight(const PointLight& pointLight, const QVector3D& position);
            void clearPointLights();
            void setMaterial(const Material& material);

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShaderProgram* _program;

            GLint _position;
            GLint _normal;
            GLint _texcoord;
            GLint _bones1;
            GLint _bones2;
            GLint _weights1;
            GLint _weights2;

            RenderUniforms _render;
            ArmatureUniforms _armature;
            LightingUniforms _lighting;
            MaterialUniforms _material;

            GLint _sampler;

            int _pointLights;
    };

}

#endif // SAHARA_SCENEPROGRAM_H
