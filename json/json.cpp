#include "json.h"

Sahara::JSON::JSON()
{

}

QJsonObject Sahara::JSON::fromTransform(const Sahara::Transform& transform)
{
    QJsonObject object;

    object["_type"] = "Transform";

    object["rotation"] = JSON::fromQuaternion(transform._rotation);
    object["translation"] = JSON::fromVector3D(transform._translation);

    return object;
}

Sahara::Transform Sahara::JSON::toTransform(const QJsonObject& object)
{
    assert(object["_type"] == "Transform");

    QQuaternion rotation = JSON::toQuaternion(object["rotation"].toArray());
    QVector3D translation = JSON::toVector3D(object["translation"].toArray());

    return Transform(rotation, translation);
}

QJsonObject Sahara::JSON::fromBone(const Sahara::Bone* bone)
{
    QJsonObject object;

    object["_type"] = "Bone";

    object["id"] = bone->_id;
    object["name"] = bone->_name;
    object["transform"] = fromTransform(bone->_transform);

    QJsonArray children;
    for (int i = 0; i < bone->_children.size(); i++) {
        children.append( fromBone(bone->_children.at(i)) );
    }

    return object;
}

Sahara::Bone*Sahara::JSON::toBone(const QJsonObject& object)
{
    assert(object["_type"] == "Bone");

    QString id = object["id"].toString();
    QString name = object["name"].toString();
    Transform transform = toTransform(object["transform"].toObject());

    Bone* bone = new Bone(nullptr, id, name, transform);

    QJsonArray children = object["children"].toArray();
    for (int i = 0; i < children.size(); i++) {
        bone->addChild( toBone(children.at(i).toObject()) );
    }

    return bone;
}

QJsonObject Sahara::JSON::fromAnimationKeyframe(const Sahara::Animation::Keyframe& keyframe)
{
    QJsonObject object;

    object["_type"] = "AnimationKeyframe";

    object["time"] = static_cast<double>(keyframe.time);
    object["transform"] = fromTransform(keyframe.transform);

    return object;
}

Sahara::Animation::Keyframe Sahara::JSON::toAnimationKeyframe(const QJsonObject& object)
{
    assert(object["_type"] == "AnimationKeyframe");

    Animation::Keyframe keyframe;
    keyframe.time = static_cast<float>(object["time"].toDouble());
    keyframe.transform = toTransform(object["transform"].toObject());

    return keyframe;
}

QJsonObject Sahara::JSON::fromAnimation(const Sahara::Animation* animation)
{
    QJsonObject object;

    object["_type"] = "Animation";

    object["bone"] = animation->_bone->id();

    QJsonArray keyframes;
    for (int i = 0; i < animation->_keyframes.size(); i++) {
        keyframes.append( fromAnimationKeyframe(animation->_keyframes.at(i)) );
    }
    object["keyframes"] = keyframes;

    return object;
}

Sahara::Animation* Sahara::JSON::toAnimation(const QJsonObject& object, Armature& armature)
{
    assert(object["_type"] == "Animation");

    QString boneId = object["bone"].toString();
    Bone* bone = armature.getBoneById(boneId);

    QJsonArray keyframes = object["keyframes"].toArray();
    QList<Animation::Keyframe> animationKeyframes;
    for (int i = 0; i < keyframes.size(); i++) {
        animationKeyframes.append( toAnimationKeyframe(keyframes.at(i).toObject()) );
    }

    return new Animation(bone, animationKeyframes);
}

QJsonObject Sahara::JSON::fromAnimationClip(const Sahara::AnimationClip* animationClip)
{
    QJsonObject object;

    object["_type"] = "AnimationClip";

    object["name"] = animationClip->_name;

    QJsonArray animations;
    for (int i = 0; i < animationClip->_animations.size(); i++) {
        animations.append( fromAnimation(animationClip->_animations.at(i)) );
    }

    object["animations"] = animations;

    return object;
}

Sahara::AnimationClip*Sahara::JSON::toAnimationClip(const QJsonObject& object, Armature& armature)
{
    assert(object["_type"] == "AnimationClip");

    QString name = object["name"].toString();

    QJsonArray animations = object["animations"].toArray();
    QList<Animation*> clipAnimations;
    for (int i = 0; i < animations.size(); i++) {
        clipAnimations.append( toAnimation(animations.at(i).toObject(), armature) );
    }
}

QJsonObject Sahara::JSON::fromArmature(const Sahara::Armature* armature)
{
    QJsonObject object;

    object["_type"] = "Armature";

    object["root"] = fromBone(armature->_root);

    return object;
}

Sahara::Armature* Sahara::JSON::toArmature(const QJsonObject& object)
{
    assert(object["_type"] == "Armature");

    Bone* root = toBone(object["root"].toObject());

    return new Armature(root);
}

QJsonObject Sahara::JSON::fromCamera(const Sahara::Camera* camera)
{
    QJsonObject object;

    object["_type"] = "Camera";

    object["mode"] = camera->_mode;
    object["xfov"] = static_cast<double>(camera->_xfov);
    object["xmag"] = static_cast<double>(camera->_xmag);
    object["aspect"] = static_cast<double>(camera->_aspect);
    object["znear"] = static_cast<double>(camera->_znear);
    object["zfar"] = static_cast<double>(camera->_zfar);

    return object;
}

Sahara::Camera* Sahara::JSON::toCamera(const QJsonObject& object)
{
    assert(object["_type"] == "Camera");

    Camera::Mode mode = static_cast<Camera::Mode>(object["mode"].toInt());
    float xfov = static_cast<float>(object["xfov"].toDouble());
    float xmag = static_cast<float>(object["xmag"].toDouble());
    float aspect = static_cast<float>(object["aspect"].toDouble());
    float znear = static_cast<float>(object["znear"].toDouble());
    float zfar = static_cast<float>(object["zfar"].toDouble());

    return new Camera(mode, xfov, xmag, aspect, znear, zfar);
}

