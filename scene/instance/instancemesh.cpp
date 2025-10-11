#include "instancemesh.h"

Sahara::InstanceMesh::InstanceMesh(const MaterialDict& materials, const QMatrix4x4& transform, Mesh* mesh)
    : Instance(materials, transform)
    , _mesh(mesh)
{

}

Sahara::Volume Sahara::InstanceMesh::volume() const
{
    QVector3D lowerVertex(1000, 1000, 1000);
    QVector3D upperVertex(-1000, -1000, -1000);

    for (int s = 0; s < _mesh->count(); s++) {
        if (_mesh->surface(s).volume().lowerVertex().x() < lowerVertex.x()) {
            lowerVertex.setX(_mesh->surface(s).volume().lowerVertex().x());
        }
        if (_mesh->surface(s).volume().lowerVertex().y() < lowerVertex.y()) {
            lowerVertex.setY(_mesh->surface(s).volume().lowerVertex().y());
        }
        if (_mesh->surface(s).volume().lowerVertex().z() < lowerVertex.z()) {
            lowerVertex.setZ(_mesh->surface(s).volume().lowerVertex().z());
        }


        if (_mesh->surface(s).volume().upperVertex().x() > upperVertex.x()) {
            upperVertex.setX(_mesh->surface(s).volume().upperVertex().x());
        }
        if (_mesh->surface(s).volume().upperVertex().y() > upperVertex.y()) {
            upperVertex.setY(_mesh->surface(s).volume().upperVertex().y());
        }
        if (_mesh->surface(s).volume().upperVertex().z() > upperVertex.z()) {
            upperVertex.setZ(_mesh->surface(s).volume().upperVertex().z());
        }
    }

    return Volume(lowerVertex, upperVertex);
}

const Sahara::Mesh& Sahara::InstanceMesh::mesh() const
{
    return *_mesh;
}

Sahara::Mesh& Sahara::InstanceMesh::mesh()
{
    return *_mesh;
}
