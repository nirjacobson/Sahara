#include "bone.h"

Sahara::Bone::Bone(Sahara::Bone* const parent, const QString& id, const QString& name, const QMatrix4x4& transform)
    : _parent(parent)
    , _id(id)
    , _name(name)
    , _transform(transform)
{

}

Sahara::Bone::~Bone()
{
    for (Bone* bone : _children) {
        delete bone;
    }
}

Sahara::Bone& Sahara::Bone::parent()
{
    return *_parent;
}

QString Sahara::Bone::id() const
{
    return _id;
}

QString Sahara::Bone::name() const
{
    return _name;
}

const QMatrix4x4& Sahara::Bone::transform() const
{
    return _transform;
}

void Sahara::Bone::setTransform(const QMatrix4x4& transform)
{
    _transform = transform;
}

void Sahara::Bone::addChild(Sahara::Bone* const child)
{
    child->_parent = this;
    _children.append(child);
}

void Sahara::Bone::depthFirst(const Sahara::Bone::BoneVisitor& visitor)
{
    bool stop = false;
    BoneVisitorStopFn doStop = [&]() {
        stop = true;
    };
    depthFirstOnBone(visitor, *this, doStop, stop);
}

void Sahara::Bone::depthFirst(const Sahara::Bone::BoneVisitorConst& visitor) const
{
    bool stop = false;
    BoneVisitorStopFn doStop = [&]() {
        stop = true;
    };
    depthFirstOnBone(visitor, *this, doStop, stop);
}

void Sahara::Bone::depthFirstOnBone(const Sahara::Bone::BoneVisitor& visitor, Sahara::Bone& bone, const Sahara::Bone::BoneVisitorStopFn& stopFn, const bool& stop)
{
    visitor(bone, stopFn);

    for (Bone* childBone : bone._children) {
        if (stop) return;
        depthFirstOnBone(visitor, *childBone, stopFn, stop);
    }
}

void Sahara::Bone::depthFirstOnBone(const Sahara::Bone::BoneVisitorConst& visitor, const Sahara::Bone& bone, const Sahara::Bone::BoneVisitorStopFn& stopFn, const bool& stop) const
{
    visitor(bone, stopFn);

    for (Bone* childBone : bone._children) {
        if (stop) return;
        depthFirstOnBone(visitor, *childBone, stopFn, stop);
    }
}
