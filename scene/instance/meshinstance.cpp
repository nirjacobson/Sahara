#include "meshinstance.h"

Sahara::MeshInstance::MeshInstance(const Sahara::MaterialDict& materials, const QMatrix4x4& transform, Sahara::Mesh* mesh)
    : Instance(materials, transform)
    , _mesh(mesh)
{

}

const Sahara::Mesh& Sahara::MeshInstance::mesh() const
{
    return *_mesh;
}

Sahara::Mesh& Sahara::MeshInstance::mesh()
{
    return *_mesh;
}
