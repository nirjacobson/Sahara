#include "mesh.h"

Sahara::Mesh::Mesh()
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

const Sahara::Source&Sahara::Mesh::source(const QString& name) const
{
    return *_sources[name];
}

void Sahara::Mesh::addSource(const QString& name, Sahara::Source* source)
{
    _sources[name] = source;
}

int Sahara::Mesh::surfaces() const
{
    return _surfaces.size();
}

Sahara::Surface&Sahara::Mesh::surface(const int i)
{
    return *_surfaces.at(i);
}

void Sahara::Mesh::addSurface(Sahara::Surface* surface)
{
    _surfaces.append(surface);
}
