#include "instancemesh.h"

Sahara::InstanceMesh::InstanceMesh(const MaterialDict& materials, const QMatrix4x4& transform, Mesh* mesh)
    : Instance(materials, transform)
    , _mesh(mesh)
{

}

const Sahara::Mesh& Sahara::InstanceMesh::mesh() const
{
    return *_mesh;
}

Sahara::Mesh& Sahara::InstanceMesh::mesh()
{
    return *_mesh;
}
