#ifndef SAHARA_JSON_H
#define SAHARA_JSON_H

#include <QJsonObject>
#include <QJsonArray>
#include <QMatrix4x4>

#include "common/transform.h"
#include "common/volume.h"
#include "scene/asset/bone.h"
#include "scene/asset/animation.h"
#include "scene/asset/animationclip.h"
#include "scene/asset/armature.h"
#include "scene/asset/camera.h"
#include "scene/asset/controller.h"
#include "scene/model.h"
#include "scene/asset/image.h"
#include "scene/asset/light/pointlight.h"
#include "scene/asset/mesh/source.h"
#include "scene/asset/mesh/surface.h"
#include "scene/asset/mesh/mesh.h"
#include "scene/instance/instancemesh.h"
#include "scene/instance/instancecontroller.h"
#include "scene/node/node.h"
#include "scene/scene.h"

namespace Sahara {

    class JSON
    {
        public:
            JSON();

            static QJsonArray fromVector3D(const QVector3D& vector);
            static QVector3D toVector3D(const QJsonArray& array);
            static QJsonArray fromVector4D(const QVector4D& vector);
            static QVector4D toVector4D(const QJsonArray& array);
            static QJsonArray fromQuaternion(const QQuaternion& quaternion);
            static QQuaternion toQuaternion(const QJsonArray& array);
            static QJsonArray fromMatrix4x4(const QMatrix4x4& matrix);
            static QMatrix4x4 toMatrix4x4(const QJsonArray& array);
            static QJsonArray fromColor(const QColor& color);
            static QColor toColor(const QJsonArray& array);

            static QJsonObject fromTransform(const Transform& transform);
            static Transform toTransform(const QJsonObject& object);

            static QJsonObject fromBone(const Bone* bone);
            static Bone* toBone(const QJsonObject& object);

            static QJsonObject fromAnimationKeyframe(const Animation::Keyframe& keyframe);
            static Animation::Keyframe toAnimationKeyframe(const QJsonObject& object);
            static QJsonObject fromAnimation(const Animation* animation);
            static Animation* toAnimation(const QJsonObject& object, Armature& armature);

            static QJsonObject fromAnimationClip(const AnimationClip* animationClip);
            static AnimationClip* toAnimationClip(const QJsonObject& object, Model& model);

            static QJsonObject fromArmature(const Armature* armature);
            static Armature* toArmature(const QJsonObject& object);

            static QJsonObject fromCamera(const Camera* camera);
            static Camera* toCamera(const QJsonObject& object);

            static QJsonObject fromController(const Controller* controller);
            static Controller* toController(const QJsonObject& object, const Model& model);

            static QJsonObject fromImage(const Image* image);
            static Image* toImage(const QJsonObject& object);

            static QJsonObject fromMaterial(const Material* material);
            static Material* toMaterial(const QJsonObject& object, const Model& model);

            static QJsonObject fromPointLight(const PointLight* pointLight);
            static PointLight* toPointLight(const QJsonObject& object);

            static QJsonObject fromSource(const Source* source);
            static Source* toSource(const QJsonObject& object);

            static QJsonObject fromSurfaceInput(const Surface::Input& input);
            static Surface::Input toSurfaceInput(const QJsonObject& object);

            static QJsonObject fromSurface(const Surface* surface);
            static Surface* toSurface(const QJsonObject& object, const Mesh& mesh);

            static QJsonObject fromMesh(const Mesh* mesh);
            static Mesh* toMesh(const QJsonObject& object);

            static QJsonObject fromInstanceMesh(const InstanceMesh* instanceMesh);
            static InstanceMesh* toInstanceMesh(const QJsonObject& object, const Model& model);

            static QJsonObject fromInstanceController(const InstanceController* instanceController);
            static InstanceController* toInstanceController(const QJsonObject& object, const Model& model);

            static QJsonObject fromVolume(const Volume& volume);
            static Volume toVolume(const QJsonObject& object);

            static QJsonObject fromModel(const Model* model);
            static Model* toModel(const QJsonObject& object);

            static QJsonObject fromNode(const Node* node);
            static Node* toNode(const QJsonObject& object, const QMap<QString, NodeItem*>& items);

            static QJsonObject fromScene(const Scene* scene);
            static Scene* toScene(const QJsonObject& object);

        private:
            static QJsonObject fromInstance(const Instance* instance);
            static void toInstance(const QJsonObject& object, Instance* instance, const Model& model);
    };

}

#endif // SAHARA_JSON_H
