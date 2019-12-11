#ifndef SAHARA_JSON_H
#define SAHARA_JSON_H

#include <QJsonObject>
#include <QJsonArray>
#include <QMatrix4x4>

#include "common/transform.h"
#include "scene/asset/bone.h"
#include "scene/asset/animation.h"
#include "scene/asset/animationclip.h"
#include "scene/asset/armature.h"
#include "scene/asset/camera.h"

namespace Sahara {

    class JSON
    {
        public:
            JSON();

            static QJsonArray fromVector3D(const QVector3D& vector);
            static QVector3D toVector3D(const QJsonArray& array);
            static QJsonArray fromVector4D(const QVector4D& vector);
            static QVector3D toVector4D(const QJsonArray& array);
            static QJsonArray fromQuaternion(const QQuaternion& vector);
            static QQuaternion toQuaternion(const QJsonArray& array);
            static QJsonArray fromMatrix4x4(const QMatrix4x4& matrix);
            static QMatrix4x4 toMatrix4x4(const QJsonArray& array);

            static QJsonObject fromTransform(const Transform& transform);
            static Transform toTransform(const QJsonObject& object);

            static QJsonObject fromBone(const Bone* bone);
            static Bone* toBone(const QJsonObject& object);

            static QJsonObject fromAnimationKeyframe(const Animation::Keyframe& keyframe);
            static Animation::Keyframe toAnimationKeyframe(const QJsonObject& object);
            static QJsonObject fromAnimation(const Animation* animation);
            static Animation* toAnimation(const QJsonObject& object, Armature& armature);

            static QJsonObject fromAnimationClip(const AnimationClip* animationClip);
            static AnimationClip* toAnimationClip(const QJsonObject& object, Armature& armature);

            static QJsonObject fromArmature(const Armature* armature);
            static Armature* toArmature(const QJsonObject& object);

            static QJsonObject fromCamera(const Camera* camera);
            static Camera* toCamera(const QJsonObject& object);
    };

}

#endif // SAHARA_JSON_H
