#include "vulkanvertexbuffer.h"

Sahara::VulkanVertexBuffer::VulkanVertexBuffer(QVulkanWindow* window)
    : _window(window)
    , _haveBuffer(false)
{

}

Sahara::VulkanVertexBuffer::~VulkanVertexBuffer()
{
    if (_haveBuffer) {
        QVulkanDeviceFunctions* deviceFunctions = _window->vulkanInstance()->deviceFunctions(_window->device());
        deviceFunctions->vkQueueWaitIdle(_window->graphicsQueue());
        deviceFunctions->vkDestroyBuffer(_window->device(), _buffer, nullptr);
        deviceFunctions->vkFreeMemory(_window->device(), _memory, nullptr);
    }
}

VkBuffer Sahara::VulkanVertexBuffer::buffer() const
{
    return _buffer;
}

void Sahara::VulkanVertexBuffer::write(const float* const data, const uint32_t size, int stride)
{
    QVulkanDeviceFunctions* deviceFunctions = _window->vulkanInstance()->deviceFunctions(_window->device());

    _size = size;
    _stride = stride;

    VkBuffer oldBuffer = _buffer;
    VkDeviceMemory oldMemory = _memory;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanUtil::createBuffer(_window, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, _window->hostVisibleMemoryIndex(), stagingBuffer, stagingBufferMemory);

    void* stagingMapped;
    deviceFunctions->vkMapMemory(_window->device(), stagingBufferMemory, 0, size, 0, &stagingMapped);
    memcpy(stagingMapped, data, size);
    deviceFunctions->vkUnmapMemory(_window->device(), stagingBufferMemory);

    VulkanUtil::createBuffer(_window, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, _window->deviceLocalMemoryIndex(), _buffer, _memory);
    VulkanUtil::copyBuffer(_window, stagingBuffer, _buffer, size);

    deviceFunctions->vkDestroyBuffer(_window->device(), stagingBuffer, nullptr);
    deviceFunctions->vkFreeMemory(_window->device(), stagingBufferMemory, nullptr);

    if (_haveBuffer) {
        deviceFunctions->vkQueueWaitIdle(_window->graphicsQueue());
        deviceFunctions->vkDestroyBuffer(_window->device(), oldBuffer, nullptr);
        deviceFunctions->vkFreeMemory(_window->device(), oldMemory, nullptr);
    }

    _haveBuffer = true;
}

uint32_t Sahara::VulkanVertexBuffer::count() const
{
    return _size / (_stride * sizeof(float));
}

