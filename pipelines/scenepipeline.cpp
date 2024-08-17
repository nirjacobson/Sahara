#include "scenepipeline.h"

ScenePipeline::~ScenePipeline()
{
    QVulkanDeviceFunctions* funcs = _vulkanWindow->vulkanInstance()->deviceFunctions(_vulkanWindow->device());

    funcs->vkDestroySampler(_vulkanWindow->device(), _sampler, nullptr);
}

ScenePipeline::ScenePipeline(QVulkanWindow *vulkanWindow, VkPolygonMode polygonMode)
    : Pipeline(vulkanWindow, "scene.vert.spv", "scene.frag.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, polygonMode)
    , _vulkanWindow(vulkanWindow)
{
    VulkanUtil::createSampler(vulkanWindow, _sampler);
}

void ScenePipeline::init()
{
    Pipeline::init<PushConstants>();
}

uint32_t ScenePipeline::binding(const QString &vertexAttribute) const
{
    QList<QString> attributes = { "position", "normal", "texcoord" };

    return attributes.indexOf(vertexAttribute);
}

QList<VkDescriptorSet> ScenePipeline::createImageDescriptorSets(VkImageView imageView)
{
    return Pipeline::createImageDescriptorSets(3, 0, _sampler, imageView);
}

QList<VkVertexInputBindingDescription> ScenePipeline::getVertexInputBindingDescriptions() const
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

    VkVertexInputBindingDescription texcoordDesc{
        .binding = 2,
        .stride = sizeof(Vertex::texcoord),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return QList<VkVertexInputBindingDescription>{ positionDesc, normalDesc, texcoordDesc };
}

QList<QList<VkDescriptorSetLayoutBinding>> ScenePipeline::getDescriptorSetLayoutBindings() const
{
    VkDescriptorSetLayoutBinding renderLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding lightingLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding materialLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding samplerLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    return QList<QList<VkDescriptorSetLayoutBinding>>{ QList<VkDescriptorSetLayoutBinding>{ renderLayoutBinding },
                                                       QList<VkDescriptorSetLayoutBinding>{ lightingLayoutBinding},
                                                       QList<VkDescriptorSetLayoutBinding>{ materialLayoutBinding },
                                                       QList<VkDescriptorSetLayoutBinding>{ samplerLayoutBinding } };
}

QList<VkVertexInputAttributeDescription> ScenePipeline::getVertexInputAttributeDescriptions() const
{
    QList<VkVertexInputAttributeDescription> attributeDescriptions(3);

    attributeDescriptions[0] = {
        .location = 0,
        .binding = 0,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = 0
    };

    attributeDescriptions[1] = {
        .location = 1,
        .binding = 1,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = 0
    };

    attributeDescriptions[2] = {
        .location = 2,
        .binding = 2,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = 0
    };

    return attributeDescriptions;
}
