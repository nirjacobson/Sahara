#include "model.h"

Sahara::Model::Model()
{

}

Sahara::Model::~Model()
{
    for (Sahara::Image* image : _images.values()) {
        delete image;
    }
    for (Sahara::Material* material : _materials.values()) {
        delete material;
    }
    for (Sahara::Mesh* mesh : _meshes.values()) {
        delete mesh;
    }
    delete _armature;
    for (Sahara::Instance* instance : _instances) {
        delete instance;
    }
    for (Sahara::AnimationClip* animationClip : _animationClips.values()) {
        delete animationClip;
    }
}

Sahara::Volume Sahara::Model::volume() const
{
    return _volume;
}

const Sahara::ImageDict& Sahara::Model::images() const
{
    return _images;
}

const Sahara::MaterialDict& Sahara::Model::materials() const
{
    return _materials;
}

const Sahara::MeshDict& Sahara::Model::meshes() const
{
    return _meshes;
}

const Sahara::ControllerDict& Sahara::Model::controllers() const
{
    return _controllers;
}

const QList<Sahara::Instance*> Sahara::Model::instances() const {
    return _instances;
}

const Sahara::AnimationDict&Sahara::Model::animations() const
{
    return _animations;
}

const Sahara::AnimationClipDict& Sahara::Model::animationClips() const
{
    return _animationClips;
}

const Sahara::Armature& Sahara::Model::armature() const
{
    return *_armature;
}

QStringList Sahara::Model::animationClipNames() const
{
    QStringList names;

    for (auto it = _animationClips.begin(); it != _animationClips.end(); it++)
        names.append(it.value()->name());

    return names;
}

void Sahara::Model::setAnimationClip(const QString& name)
{
    for (auto it = _animationClips.begin(); it != _animationClips.end(); it++) {
        if (it.value()->name() == name) {
            _animationClip = it.value();
            return;
        }
    }
}

QString Sahara::Model::animationClip() const
{
    return _animationClip ? _animationClip->name() : "";
}

int Sahara::Model::triangles() const
{
    int triangles = 0;
    for (Sahara::Mesh* mesh : _meshes.values()) {
        triangles += mesh->triangles();
    }

    return triangles;
}

void Sahara::Model::animate(const float time)
{
    if (_animationClip != nullptr) {
        _animationClip->apply(time);
    }
}

