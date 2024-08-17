#include "vulkanmesh.h"

Sahara::VulkanMesh::VulkanMesh(QVulkanWindow* window, const QString& id)
    : Mesh(id)
    , _vulkanWindow(window)
{

}

Sahara::VulkanMesh::~VulkanMesh()
{
    for (const Source* source : _sources.values()) {
        delete source;
    }
    for (const Surface* surface : _surfaces) {
        delete surface;
    }
}

Sahara::Surface& Sahara::VulkanMesh::surface(const int i)
{
    return *dynamic_cast<Sahara::VulkanSurface*>(_surfaces.at(i));
}

Sahara::Surface& Sahara::VulkanMesh::add(const QString& material)
{
    VulkanSurface* surface = new VulkanSurface(_vulkanWindow, _sources, material);
    _surfaces.append(surface);

    return *surface;
}
