#include "vulkanutil.h"

void VulkanUtil::createBuffer(QVulkanWindow* window, VkDeviceSize size, VkBufferUsageFlags usage, uint32_t memoryTypeIndex, VkBuffer &buffer, VkDeviceMemory &memory)
{
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };

    if (funcs->vkCreateBuffer(window->device(), &bufferInfo, NULL, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer.");
    }

    VkMemoryRequirements memRequirements;
    funcs->vkGetBufferMemoryRequirements(window->device(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = memoryTypeIndex
    };

    if (funcs->vkAllocateMemory(window->device(), &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory.\n");
        assert(false);
    }

    funcs->vkBindBufferMemory(window->device(), buffer, memory, 0);
}

VkCommandBuffer VulkanUtil::beginSingleTimeCommands(QVulkanWindow* window){
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    VkCommandBufferAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = window->graphicsCommandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer commandBuffer;
    funcs->vkAllocateCommandBuffers(window->device(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    funcs->vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanUtil::endSingleTimeCommands(QVulkanWindow* window, VkCommandBuffer commandBuffer) {
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    funcs->vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer
    };

    funcs->vkQueueSubmit(window->graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    funcs->vkQueueWaitIdle(window->graphicsQueue());

    funcs->vkFreeCommandBuffers(window->device(), window->graphicsCommandPool(), 1, &commandBuffer);
}

void VulkanUtil::copyBuffer(QVulkanWindow *window, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(window);

    VkBufferCopy copyRegion = {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size
    };
    funcs->vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(window, commandBuffer);
}

void VulkanUtil::createImage(QVulkanWindow *window, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage &image, VkDeviceMemory &imageMemory)
{
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    VkImageCreateInfo imageInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent = {
            .width = width,
            .height = height,
            .depth = 1
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = tiling,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    if (funcs->vkCreateImage(window->device(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("could not create image.");
    }

    VkMemoryRequirements memRequirements;
    funcs->vkGetImageMemoryRequirements(window->device(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = window->deviceLocalMemoryIndex()
    };

    if (funcs->vkAllocateMemory(window->device(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory.");
    }

    funcs->vkBindImageMemory(window->device(), image, imageMemory, 0);
}

VkImageView VulkanUtil::createImageView(QVulkanWindow *window, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    VkImageViewCreateInfo viewInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange = {
            .aspectMask = aspectFlags,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    VkImageView imageView;
    if (funcs->vkCreateImageView(window->device(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("could not create image view.");
    }

    return imageView;
}

void VulkanUtil::transitionImageLayout(QVulkanWindow *window, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(window);

    VkImageMemoryBarrier barrier{
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = 0,
        .oldLayout = oldLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::runtime_error("unsupported layout transition.");
    }

    funcs->vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    endSingleTimeCommands(window, commandBuffer);
}

void VulkanUtil::copyBufferToImage(QVulkanWindow *window, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(window);

    VkBufferImageCopy region{
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
        .imageOffset = {
            .x = 0,
            .y = 0,
            .z = 0
        },
        .imageExtent = {
            .width = width,
            .height = height,
            .depth = 1
        }
    };

    funcs->vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(window, commandBuffer);
}

void VulkanUtil::createSampler(QVulkanWindow *window, VkSampler &sampler)
{
    QVulkanDeviceFunctions* funcs = window->vulkanInstance()->deviceFunctions(window->device());

    const VkPhysicalDeviceProperties* properties = window->physicalDeviceProperties();

    VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias = 0.0f,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = properties->limits.maxSamplerAnisotropy,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0f,
        .maxLod = 0.0f,
        .unnormalizedCoordinates = VK_FALSE
    };

    if (funcs->vkCreateSampler(window->device(), &samplerInfo, NULL, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler.");
    }
}
