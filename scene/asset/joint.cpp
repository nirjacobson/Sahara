#include "joint.h"

Sahara::Joint::Joint(Sahara::Joint* const parent, const QString& id, const QString& name, const Transform& transform)
    : _parent(parent)
    , _id(id)
    , _name(name)
    , _transform(transform)
{

}

Sahara::Joint::~Joint()
{
    for (Joint* joint : _children) {
        delete joint;
    }
}

const Sahara::Joint& Sahara::Joint::parent() const
{
    return *_parent;
}

Sahara::Joint& Sahara::Joint::parent()
{
    return *_parent;
}

QString Sahara::Joint::id() const
{
    return _id;
}

QString Sahara::Joint::name() const
{
    return _name;
}

void Sahara::Joint::addChild(Sahara::Joint* const child)
{
    child->_parent = this;
    _children.append(child);
}

const Sahara::Transform& Sahara::Joint::transform() const
{
    return _transform;
}

void Sahara::Joint::setTransform(const Sahara::Transform& transform)
{
    _transform = transform;
}

bool Sahara::Joint::depthFirst(const Sahara::Joint::JointVisitor& visitor)
{
    if (visitor(*this)) {
        return true;
    }

    for (Joint* childJoint : _children) {
        if (childJoint->depthFirst(visitor)) {
            return true;
        }
    }

    return false;
}

bool Sahara::Joint::depthFirst(const Sahara::Joint::JointVisitorConst& visitor) const
{
    if (visitor(*this)) {
        return true;
    }

    for (const Joint* childJoint : _children) {
        if (childJoint->depthFirst(visitor)) {
            return true;
        }
    }

    return false;
}
