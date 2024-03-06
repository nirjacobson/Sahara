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

void ScenePipeline::create()
{
    Pipeline::create<PushConstants>();
}

uint32_t ScenePipeline::binding(const QString &vertexAttribute) const
{
    QList<QString> attributes = { "position", "normal", "texcoord", "joints", "weights" };

    return attributes.indexOf(vertexAttribute);
}

QList<VkDescriptorSet> ScenePipeline::createImageDescriptorSets(VkImageView imageView)
{
    return Pipeline::createImageDescriptorSets(4, _sampler, imageView);
}

QList<VkVertexInputBindingDescription> ScenePipeline::getVertexInputBindingDescriptions()
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

    VkVertexInputBindingDescription jointsDesc{
        .binding = 3,
        .stride = sizeof(Vertex::joints),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    VkVertexInputBindingDescription weightsDesc{
        .binding = 4,
        .stride = sizeof(Vertex::weights),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return QList<VkVertexInputBindingDescription>{ positionDesc, normalDesc, texcoordDesc, jointsDesc, weightsDesc };
}

QList<VkDescriptorSetLayoutBinding> ScenePipeline::getDescriptorSetLayoutBindings()
{
    VkDescriptorSetLayoutBinding renderLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding armatureLayoutBinding{
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding lightingLayoutBinding{
        .binding = 2,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding materialLayoutBinding{
        .binding = 3,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutBinding samplerLayoutBinding{
        .binding = 4,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };

    return QList<VkDescriptorSetLayoutBinding>{ renderLayoutBinding, armatureLayoutBinding, lightingLayoutBinding, materialLayoutBinding, samplerLayoutBinding };
}

QList<VkVertexInputAttributeDescription> ScenePipeline::getVertexInputAttributeDescriptions()
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
        .binding = 0,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(Vertex, normal)
    };

    attributeDescriptions[2] = {
        .location = 2,
        .binding = 0,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = offsetof(Vertex, texcoord)
    };

    attributeDescriptions[3] = {
                                .location = 3,
        .binding = 0,
        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
        .offset = offsetof(Vertex, joints)
    };

    attributeDescriptions[4] = {
        .location = 3,
        .binding = 0,
        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
        .offset = offsetof(Vertex, weights)
    };

    return attributeDescriptions;
}
