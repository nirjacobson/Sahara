#include "displaypipeline.h"

DisplayPipeline::DisplayPipeline(QVulkanWindow* vulkanWindow)
    : Pipeline(vulkanWindow, "display.vert.spv", "display.frag.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_LINE)
{

}

void DisplayPipeline::init()
{
    Pipeline::init<PushConstants>();
}

uint32_t DisplayPipeline::binding(const QString &vertexAttribute) const
{
    QList<QString> attributes = { "position", "normal" };

    return attributes.indexOf(vertexAttribute);
}

QList<VkVertexInputBindingDescription> DisplayPipeline::getVertexInputBindingDescriptions()
{
    VkVertexInputBindingDescription positionDesc{
        .binding = 0,
        .stride = sizeof(Vertex::position),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    VkVertexInputBindingDescription normalDesc{
        .binding = 1,
        .stride = sizeof(Vertex::normal),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return QList<VkVertexInputBindingDescription>{ positionDesc, normalDesc };
}

QList<QList<VkDescriptorSetLayoutBinding> > DisplayPipeline::getDescriptorSetLayoutBindings()
{
    VkDescriptorSetLayoutBinding vertexRenderLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    return { QList<VkDescriptorSetLayoutBinding>{ vertexRenderLayoutBinding } };
}

QList<VkVertexInputAttributeDescription> DisplayPipeline::getVertexInputAttributeDescriptions()
{
    QList<VkVertexInputAttributeDescription> attributeDescriptions(2);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, normal);

    return attributeDescriptions;
}
