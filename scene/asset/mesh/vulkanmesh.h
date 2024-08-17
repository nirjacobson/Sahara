#ifndef SAHARA_VULKAN_MESH_H
#define SAHARA_VULKAN_MESH_H

#include <QList>
#include <QVulkanWindow>

#include "mesh.h"
#include "source.h"
#include "vulkansurface.h"

namespace Sahara
{
    class VulkanMesh : public Mesh
    {
        friend class JSON;

        public:
            VulkanMesh(QVulkanWindow *window, const QString& id);
            ~VulkanMesh();

            Surface& surface(const int i) override;
            Surface& add(const QString& material) override;

        private:
            QVulkanWindow* _vulkanWindow;
    };
}

#endif // SAHARA_VULKAN_MESH_H
