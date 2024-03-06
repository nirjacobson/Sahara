#include "gridpipeline.h"

GridPipeline::GridPipeline(QVulkanWindow* vulkanWindow, VkPrimitiveTopology topology)
    : Pipeline(vulkanWindow, "grid.vert.spv", "grid.frag.spv", topology)
{

}

void GridPipeline::create()
{
    Pipeline::createStandard();
}

uint32_t GridPipeline::binding(const QString &vertexAttribute) const
{
    QList<QString> attributes = { "position", "color" };

    return attributes.indexOf(vertexAttribute);
}

QList<VkVertexInputBindingDescription> GridPipeline::getVertexInputBindingDescriptions()
{
    VkVertexInputBindingDescription positionDesc{
        .binding = 0,
        .stride = sizeof(Vertex::position),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    VkVertexInputBindingDescription colorDesc{
        .binding = 1,
        .stride = sizeof(Vertex::color),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return QList<VkVertexInputBindingDescription>{ positionDesc, colorDesc };
}

QList<VkDescriptorSetLayoutBinding> GridPipeline::getDescriptorSetLayoutBindings()
{
    VkDescriptorSetLayoutBinding vertexRenderLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    return QList<VkDescriptorSetLayoutBinding>{ vertexRenderLayoutBinding };
}

QList<VkVertexInputAttributeDescription> GridPipeline::getVertexInputAttributeDescriptions()
{
    QList<VkVertexInputAttributeDescription> attributeDescriptions(2);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}
