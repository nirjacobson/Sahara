#ifndef DISPLAYPIPELINE_H
#define DISPLAYPIPELINE_H

#include "pipeline.h"

class DisplayPipeline : public Pipeline
{
public:
    struct Render {
        alignas(16) float inverseCamera[16];
        alignas(16) float projection[16];
    };

    struct PushConstants {
        alignas(16) float modelView[16];
        alignas(16) float cameraPosition[3];
        alignas(16) int focus;
    };

    DisplayPipeline(QVulkanWindow* vulkanWindow);

    void init();
    uint32_t binding(const QString &vertexAttribute) const;

private:

    struct Vertex {
        float position[3];
        float normal[3];
    };

    QList<VkVertexInputBindingDescription> getVertexInputBindingDescriptions() const;
    QList<QList<VkDescriptorSetLayoutBinding>> getDescriptorSetLayoutBindings() const;
    QList<VkVertexInputAttributeDescription> getVertexInputAttributeDescriptions() const;
};

#endif // DISPLAYPIPELINE_H
