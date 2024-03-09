#include "panelpipeline.h"

PanelPipeline::PanelPipeline(QVulkanWindow *vulkanWindow)
    : Pipeline(vulkanWindow, "panel.vert.spv", "panel.frag.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, VK_POLYGON_MODE_FILL)
{

}

void PanelPipeline::init()
{
    Pipeline::init<PushConstants>();
}

uint32_t PanelPipeline::binding(const QString &vertexAttribute) const
{
    QList<QString> attributes = { "position", "texcoord" };

    return attributes.indexOf(vertexAttribute);
}

QList<VkVertexInputBindingDescription> PanelPipeline::getVertexInputBindingDescriptions() const
{
    VkVertexInputBindingDescription positionDesc{
        .binding = 0,
        .stride = sizeof(Vertex::position),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    VkVertexInputBindingDescription texcoordDesc{
        .binding = 1,
        .stride = sizeof(Vertex::texcoord),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return QList<VkVertexInputBindingDescription>{ positionDesc, texcoordDesc };
}

QList<QList<VkDescriptorSetLayoutBinding> > PanelPipeline::getDescriptorSetLayoutBindings() const
{
    VkDescriptorSetLayoutBinding vertexRenderLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding samplerLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    return { QList<VkDescriptorSetLayoutBinding>{ vertexRenderLayoutBinding },
             QList<VkDescriptorSetLayoutBinding>{ samplerLayoutBinding } };
}

QList<VkVertexInputAttributeDescription> PanelPipeline::getVertexInputAttributeDescriptions() const
{
    QList<VkVertexInputAttributeDescription> attributeDescriptions(2);

    attributeDescriptions[0] = {
        .location = 0,
        .binding = 0,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = 0
    };

    attributeDescriptions[1] = {
        .location = 1,
        .binding = 1,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = 0
    };

    return attributeDescriptions;
}
