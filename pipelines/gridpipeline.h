#ifndef GRIDPIPELINE_H
#define GRIDPIPELINE_H

#include "pipeline.h"

class GridPipeline : public Pipeline
{
public:
    struct Render {
        alignas(16) float inverseCamera[16];
        alignas(16) float projection[16];
    };

    GridPipeline(QVulkanWindow *vulkanWindow, VkPrimitiveTopology topology);

    void init();
    uint32_t binding(const QString &vertexAttribute) const;

private:
    struct Vertex {
        float position[3];
        float color[3];
    };

    QList<VkVertexInputBindingDescription> getVertexInputBindingDescriptions() const;
    QList<QList<VkDescriptorSetLayoutBinding>> getDescriptorSetLayoutBindings() const;
    QList<VkVertexInputAttributeDescription> getVertexInputAttributeDescriptions() const;
};

#endif // GRIDPIPELINE_H
