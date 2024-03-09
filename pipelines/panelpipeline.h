#ifndef PANELPIPELINE_H
#define PANELPIPELINE_H

#include "pipeline.h"

class PanelPipeline : public Pipeline
{
public:
    struct Render {
        float projection[16];
    };

    struct PushConstants {
        float location[2];
    };

    PanelPipeline(QVulkanWindow *vulkanWindow);

    // Pipeline interface
public:
    void init();
    uint32_t binding(const QString &vertexAttribute) const;

private:
    struct Vertex {
        float position[2];
        float texcoord[2];
    };

    QList<VkVertexInputBindingDescription> getVertexInputBindingDescriptions() const;
    QList<QList<VkDescriptorSetLayoutBinding> > getDescriptorSetLayoutBindings() const;
    QList<VkVertexInputAttributeDescription> getVertexInputAttributeDescriptions() const;
};

#endif // PANELPIPELINE_H
