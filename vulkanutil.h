#ifndef VULKANUTIL_H
#define VULKANUTIL_H

#include <QVulkanDeviceFunctions>
#include <QVulkanWindow>

#include <stdexcept>

class VulkanUtil
{
public:
    struct UniformBuffers {
        QList<VkBuffer> buffers;
        QList<VkDeviceMemory> buffersMemory;
        QList<void*> buffersMapped;
        QList<VkDescriptorSet> bufferDescriptorSets;

        void resize(const uint32_t size) {
            buffers.resize(size);
            buffersMemory.resize(size);
            buffersMapped.resize(size);
        }
    };

    static void createBuffer(QVulkanWindow* window, VkDeviceSize size, VkBufferUsageFlags usage, uint32_t memoryTypeIndex, VkBuffer& buffer, VkDeviceMemory& memory);
    static void copyBuffer(QVulkanWindow* window, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    static void createImage(QVulkanWindow* window, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage& image, VkDeviceMemory& imageMemory);
    static VkImageView createImageView(QVulkanWindow* window, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void transitionImageLayout(QVulkanWindow* window, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    static void copyBufferToImage(QVulkanWindow* window, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    static void createSampler(QVulkanWindow* window, VkSampler& sampler);


private:
    static VkCommandBuffer beginSingleTimeCommands(QVulkanWindow* window);
    static void endSingleTimeCommands(QVulkanWindow* window, VkCommandBuffer commandBuffer);

};

#endif // VULKANUTIL_H
