#include "bone.h"

Sahara::Bone::Bone(Sahara::Bone* const parent, const QString& id, const QString& name, const Transform& transform)
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

const Sahara::Bone& Sahara::Bone::parent() const
{
    return *_parent;
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

void Sahara::Bone::addChild(Sahara::Bone* const child)
{
    child->_parent = this;
    _children.append(child);
}

const Sahara::Transform& Sahara::Bone::transform() const
{
    return _transform;
}

void Sahara::Bone::setTransform(const Sahara::Transform& transform)
{
    _transform = transform;
}

bool Sahara::Bone::depthFirst(const Sahara::Bone::BoneVisitor& visitor)
{
    if (visitor(*this)) {
        return true;
    }

    for (Bone* childBone : _children) {
        if (childBone->depthFirst(visitor)) {
            return true;
        }
    }

    return false;
}

bool Sahara::Bone::depthFirst(const Sahara::Bone::BoneVisitorConst& visitor) const
{
    if (visitor(*this)) {
        return true;
    }

    for (const Bone* childBone : _children) {
        if (childBone->depthFirst(visitor)) {
            return true;
        }
    }

    return false;
}
