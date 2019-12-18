#include "mesh.h"

Sahara::Mesh::Mesh(const QString& id)
    : Asset(id)
{

}

Sahara::Mesh::~Mesh()
{
    for (const Source* source : _sources.values()) {
        delete source;
    }
    for (const Surface* surface : _surfaces) {
        delete surface;
    }
}

QStringList Sahara::Mesh::sources() const
{
    return _sources.keys();
}

void Sahara::Mesh::add(const QString& name, Sahara::Source* source)
{
    _sources[name] = source;
}

int Sahara::Mesh::count() const
{
    return _surfaces.size();
}

Sahara::Surface&Sahara::Mesh::surface(const int i)
{
    return *_surfaces.at(i);
}

Sahara::Surface& Sahara::Mesh::add(const QString& material)
{
    Surface* surface = new Surface(_sources, material);
    _surfaces.append(surface);

    return *surface;
}

int Sahara::Mesh::triangles() const
{
    int triangles = 0;
    for (int i = 0; i < _surfaces.size(); i++) {
        triangles += _surfaces.at(i)->triangles();
    }

    return triangles;
}

