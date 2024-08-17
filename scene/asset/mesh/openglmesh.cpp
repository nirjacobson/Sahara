#include "openglmesh.h"

Sahara::OpenGLMesh::OpenGLMesh(const QString& id)
    : Mesh(id)
{

}

Sahara::OpenGLMesh::~OpenGLMesh()
{
    for (const Source* source : _sources.values()) {
        delete source;
    }
    for (const Surface* surface : _surfaces) {
        delete surface;
    }
}

Sahara::Surface& Sahara::OpenGLMesh::surface(const int i)
{
    return *dynamic_cast<Sahara::OpenGLSurface*>(_surfaces.at(i));
}

Sahara::Surface& Sahara::OpenGLMesh::add(const QString& material)
{
    OpenGLSurface* surface = new OpenGLSurface(_sources, material);
    _surfaces.append(surface);

    return *surface;
}

