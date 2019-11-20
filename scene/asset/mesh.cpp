#include "mesh.h"

Sahara::Mesh::Mesh(const QList<Sahara::Surface>& surfaces)
    : _surfaces(surfaces)
{

}

const QList<Sahara::Surface>& Sahara::Mesh::surfaces() const
{
    return _surfaces;
}

QList<Sahara::Surface>& Sahara::Mesh::surfaces()
{
    return _surfaces;
}
