#include "instance.h"

Sahara::Instance::Instance(const Sahara::MaterialDict& materials, const QMatrix4x4& transform)
    : _materials(materials)
    , _transform(transform)
{

}

Sahara::Instance::~Instance()
{

}

QStringList Sahara::Instance::materials() const
{
    return _materials.keys();
}

const Sahara::Material& Sahara::Instance::getMaterial(const QString& name) const
{
    return *_materials[name];
}

const QMatrix4x4& Sahara::Instance::transform() const
{
    return _transform;
}
