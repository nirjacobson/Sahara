#include "mesh.h"

Sahara::Mesh::Mesh(const QString& id)
    : Asset(id)
{

}

QStringList Sahara::Mesh::sources() const
{
    return _sources.keys();
}

void Sahara::Mesh::add(const QString& name, Source* source)
{
    _sources[name] = source;
}

int Sahara::Mesh::count() const
{
    return _surfaces.size();
}

int Sahara::Mesh::triangles() const
{
    int triangles = 0;
    for (int i = 0; i < _surfaces.size(); i++) {
        triangles += _surfaces.at(i)->triangles();
    }

    return triangles;
}
