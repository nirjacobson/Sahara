#include "armature.h"


Sahara::Armature::Armature(Sahara::Bone* const root)
    : _root(root)
{

}

Sahara::Armature::~Armature()
{
    delete _root;
}

Sahara::Bone* Sahara::Armature::getBoneById(const QString& id)
{
    Bone* bone = nullptr;
    _root->depthFirst([&](Bone& curBone, auto& stop) {
        if (curBone.id() == id) {
            bone = &curBone;
            stop();
        }
    });

    return bone;
}

const Sahara::Bone* Sahara::Armature::getBoneByName(const QString& name) const
{
    const Bone* bone = nullptr;
    _root->depthFirst([&](const Bone& curBone, auto& stop) {
        if (curBone.name() == name) {
            bone = &curBone;
            stop();
        }
    });

    return bone;
}
