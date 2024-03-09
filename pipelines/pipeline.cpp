#include "pipeline.h"

const QList<VkDynamicState> Pipeline::dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
};

Pipeline::Pipeline(QVulkanWindow* vulkanWindow, const QString& vertShaderPath, const QString& fragShaderPath, const VkPrimitiveTopology topology, const VkPolygonMode polygonMode)
    : _vulkanWindow(vulkanWindow)
    , _deviceFunctions(vulkanWindow->vulkanInstance()->deviceFunctions(vulkanWindow->device()))
    , _vertShaderPath(":/programs/spir-v/"+vertShaderPath)
    , _fragShaderPath(":/programs/spir-v/"+fragShaderPath)
    , _topology(topology)
    , _polygonMode(polygonMode)
{

}

Pipeline::~Pipeline()
{
    _deviceFunctions->vkDestroyDescriptorPool(_vulkanWindow->device(), _descriptorPool, nullptr);
    for (const auto& dsLayout : _dsLayouts) {
        _deviceFunctions->vkDestroyDescriptorSetLayout(_vulkanWindow->device(), dsLayout, nullptr);
    }
    _deviceFunctions->vkDestroyPipeline(_vulkanWindow->device(), _pipeline, nullptr);
    _deviceFunctions->vkDestroyPipelineLayout(_vulkanWindow->device(), _pipelineLayout, nullptr);
}

void Pipeline::create()
{
    createDescriptorPool();
    init();
}

QList<VkDescriptorSet> Pipeline::createBufferDescriptorSets(uint32_t set, uint32_t binding, const QList<VkBuffer>& buffers, VkDeviceSize size)
{
    uint32_t concurrentFrames = _vulkanWindow->concurrentFrameCount();
    QList<VkDescriptorSetLayout> layouts(concurrentFrames, _dsLayouts[set]);

    VkDescriptorSetAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = _descriptorPool,
        .descriptorSetCount = concurrentFrames,
        .pSetLayouts = layouts.data()
    };

    QList<VkDescriptorSet> descriptorSets(concurrentFrames);
    if (_deviceFunctions->vkAllocateDescriptorSets(_vulkanWindow->device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets.\n");
    }

    for (int i = 0; i < concurrentFrames; i++) {
        VkDescriptorBufferInfo bufferInfo = {
            bufferInfo.buffer = buffers[i],
            bufferInfo.offset = 0,
            bufferInfo.range = size
        };

        VkWriteDescriptorSet descriptorWrite{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptorSets[i],
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pImageInfo = NULL,
            .pBufferInfo = &bufferInfo,
            .pTexelBufferView = NULL
        };

        _deviceFunctions->vkUpdateDescriptorSets(_vulkanWindow->device(), 1, &descriptorWrite, 0, nullptr);
    }

    return descriptorSets;
}

QList<VkDescriptorSet> Pipeline::createImageDescriptorSets(uint32_t set, uint32_t binding, VkSampler sampler, VkImageView imageView)
{
    uint32_t concurrentFrames = _vulkanWindow->concurrentFrameCount();
    QList<VkDescriptorSetLayout> layouts(concurrentFrames, _dsLayouts[set]);

    VkDescriptorSetAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = _descriptorPool,
        .descriptorSetCount = concurrentFrames,
        .pSetLayouts = layouts.data()
    };

    QList<VkDescriptorSet> descriptorSets(concurrentFrames);
    if (_deviceFunctions->vkAllocateDescriptorSets(_vulkanWindow->device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets.\n");
    }
    for (int i = 0; i < concurrentFrames; i++) {
        VkDescriptorImageInfo imageInfo = {
            .sampler = sampler,
            .imageView = imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };

        VkWriteDescriptorSet descriptorWrite{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptorSets[i],
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &imageInfo,
            .pBufferInfo = NULL,
            .pTexelBufferView = NULL
        };

        _deviceFunctions->vkUpdateDescriptorSets(_vulkanWindow->device(), 1, &descriptorWrite, 0, NULL);
    }

    return descriptorSets;
}

void Pipeline::freeDescriptorSets(const QList<VkDescriptorSet> &descriptorSets)
{
    _deviceFunctions->vkFreeDescriptorSets(_vulkanWindow->device(), _descriptorPool, descriptorSets.size(), descriptorSets.data());
}

VkPipeline Pipeline::pipeline() const
{
    return _pipeline;
}

VkPipelineLayout Pipeline::pipelineLayout() const
{
    return _pipelineLayout;
}

VkShaderModule Pipeline::createShaderModule(const QString &name)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error(QString("Failed to read shader %1").arg(name).toStdString());
    }
    QByteArray blob = file.readAll();
    file.close();

    VkShaderModuleCreateInfo shaderInfo{};
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = blob.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t *>(blob.constData());
    VkShaderModule shaderModule;
    VkResult err = _deviceFunctions->vkCreateShaderModule(_vulkanWindow->device(), &shaderInfo, nullptr, &shaderModule);
    if (err != VK_SUCCESS) {
        throw std::runtime_error(QString("Failed to create shader module: %1").arg(err).toStdString());
    }

    return shaderModule;
}

void Pipeline::createDescriptorPool()
{
    uint32_t concurrentFrames = _vulkanWindow->concurrentFrameCount();

    QList<QList<VkDescriptorSetLayoutBinding>> layoutBindings = getDescriptorSetLayoutBindings();

    QList<VkDescriptorPoolSize> descPoolSizes;
    for (int i = 0; i < layoutBindings.size(); i++) {
        for (int j = 0; j < layoutBindings[i].size(); j++) {
            descPoolSizes.append({ layoutBindings[i][j].descriptorType, 4 * concurrentFrames * layoutBindings[i][j].descriptorCount });
        }
    }

    VkDescriptorPoolCreateInfo descPoolInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .maxSets = 16,
        .poolSizeCount = static_cast<uint32_t>(descPoolSizes.size()),
        .pPoolSizes = descPoolSizes.data()
    };

    if (_deviceFunctions->vkCreateDescriptorPool(_vulkanWindow->device(), &descPoolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("could not create descriptor pool.");
    }
}

void Pipeline::createDescriptorSetLayouts()
{
    QList<QList<VkDescriptorSetLayoutBinding>> layoutBindings = getDescriptorSetLayoutBindings();

    QList<VkDescriptorSetLayout> layouts(layoutBindings.size());

    for (int i = 0; i < layoutBindings.size(); i++) {
        VkDescriptorSetLayoutCreateInfo layoutInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(layoutBindings[i].size()),
            .pBindings = layoutBindings[i].data(),
        };

        if (_deviceFunctions->vkCreateDescriptorSetLayout(_vulkanWindow->device(), &layoutInfo, nullptr, &layouts[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout.");
        }
    }

    _dsLayouts = layouts;
}


