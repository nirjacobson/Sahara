#include "armature.h"


Sahara::Armature::Armature(const QString& id, Sahara::Joint* const root)
    : Asset(id)
    , _root(root)
{

}

Sahara::Armature::~Armature()
{
    delete _root;
}

const Sahara::Joint& Sahara::Armature::root() const
{
    return *_root;
}

Sahara::Joint* Sahara::Armature::getJointById(const QString& id)
{
    Joint* joint = nullptr;
    _root->depthFirst([&](Joint& curJoint) {
        if (curJoint.id() == id) {
            joint = &curJoint;
            return true;
        }
        return false;
    });

    return joint;
}

const Sahara::Joint* Sahara::Armature::getJointByName(const QString& name) const
{
    const Joint* joint = nullptr;
    _root->depthFirst([&](const Joint& curJoint) {
        if (curJoint.name() == name) {
            joint = &curJoint;
            return true;
        }
        return false;
    });

    return joint;
}
