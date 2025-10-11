#include "instance.h"

Sahara::Instance::Instance(const Sahara::MaterialDict& materials, const QMatrix4x4& transform)
    : _materials(materials)
    , _transform(transform)
    , _focusSurface(-1)
{

}

Sahara::Instance::~Instance()
{

}

QStringList Sahara::Instance::materials() const
{
    return _materials.keys();
}

void Sahara::Instance::addMaterial(const QString &name, Material *material)
{
    _materials.insert(name, material);
}

Sahara::Material& Sahara::Instance::getMaterial(const QString& name)
{
    return *_materials[name];
}

const QMatrix4x4& Sahara::Instance::transform() const
{
    return _transform;
}

int Sahara::Instance::focusSurface() const
{
    return _focusSurface;
}

void Sahara::Instance::setFocusSurface(const int idx)
{
    _focusSurface = idx;
}
