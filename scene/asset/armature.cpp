#include "armature.h"


Sahara::Armature::Armature(const QString& id, Sahara::Bone* const root)
    : Asset(id)
    , _root(root)
{

}

Sahara::Armature::~Armature()
{
    delete _root;
}

const Sahara::Bone& Sahara::Armature::root() const
{
    return *_root;
}

Sahara::Bone* Sahara::Armature::getBoneById(const QString& id)
{
    Bone* bone = nullptr;
    _root->depthFirst([&](Bone& curBone) {
        if (curBone.id() == id) {
            bone = &curBone;
            return true;
        }
        return false;
    });

    return bone;
}

const Sahara::Bone* Sahara::Armature::getBoneByName(const QString& name) const
{
    const Bone* bone = nullptr;
    _root->depthFirst([&](const Bone& curBone) {
        if (curBone.name() == name) {
            bone = &curBone;
            return true;
        }
        return false;
    });

    return bone;
}
