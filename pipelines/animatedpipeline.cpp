#include "animatedpipeline.h"

AnimatedPipeline::~AnimatedPipeline()
{
    QVulkanDeviceFunctions* funcs = _vulkanWindow->vulkanInstance()->deviceFunctions(_vulkanWindow->device());

    funcs->vkDestroySampler(_vulkanWindow->device(), _sampler, nullptr);
}

AnimatedPipeline::AnimatedPipeline(QVulkanWindow *vulkanWindow, VkPolygonMode polygonMode)
    : Pipeline(vulkanWindow, "scene.vert.spv", "scene.frag.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, polygonMode)
    , _vulkanWindow(vulkanWindow)
{
    VulkanUtil::createSampler(vulkanWindow, _sampler);
}

void AnimatedPipeline::init()
{
    Pipeline::init<PushConstants>();
}

uint32_t AnimatedPipeline::binding(const QString &vertexAttribute) const
{
    QList<QString> attributes = { "position", "normal", "texcoord", "joints", "weights" };

    return attributes.indexOf(vertexAttribute);
}

QList<VkDescriptorSet> AnimatedPipeline::createImageDescriptorSets(VkImageView imageView)
{
    return Pipeline::createImageDescriptorSets(3, 0, _sampler, imageView);
}

QList<VkVertexInputBindingDescription> AnimatedPipeline::getVertexInputBindingDescriptions()
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

QList<QList<VkDescriptorSetLayoutBinding>> AnimatedPipeline::getDescriptorSetLayoutBindings()
{
    VkDescriptorSetLayoutBinding renderLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding armatureLayoutBinding{
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

QList<VkVertexInputAttributeDescription> AnimatedPipeline::getVertexInputAttributeDescriptions()
{
    QList<VkVertexInputAttributeDescription> attributeDescriptions(5);

    attributeDescriptions[0] = {
        .location = 0,
        .binding = 0,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(Vertex, position)
    };

    attributeDescriptions[1] = {
        .location = 1,
        .binding = 1,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(Vertex, normal)
    };

    attributeDescriptions[2] = {
        .location = 2,
        .binding = 2,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = offsetof(Vertex, texcoord)
    };

    return attributeDescriptions;
}
