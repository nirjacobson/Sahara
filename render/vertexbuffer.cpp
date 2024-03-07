#include "vertexbuffer.h"

Sahara::VertexBuffer::VertexBuffer(QVulkanWindow* window)
    : _window(window)
    , _deviceFunctions(window->vulkanInstance()->deviceFunctions(window->device()))
    , _haveBuffer(false)
{

}

Sahara::VertexBuffer::~VertexBuffer()
{
    if (_haveBuffer) {
        _deviceFunctions->vkQueueWaitIdle(_window->graphicsQueue());
        _deviceFunctions->vkDestroyBuffer(_window->device(), _buffer, nullptr);
        _deviceFunctions->vkFreeMemory(_window->device(), _memory, nullptr);
    }
}

VkBuffer Sahara::VertexBuffer::buffer() const
{
    return _buffer;
}

void Sahara::VertexBuffer::write(const float* const data, const uint32_t size)
{
    VkBuffer oldBuffer = _buffer;
    VkDeviceMemory oldMemory = _memory;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanUtil::createBuffer(_window, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, _window->hostVisibleMemoryIndex(), stagingBuffer, stagingBufferMemory);

    void* stagingMapped;
    _deviceFunctions->vkMapMemory(_window->device(), stagingBufferMemory, 0, size, 0, &stagingMapped);
    memcpy(stagingMapped, data, size);
    _deviceFunctions->vkUnmapMemory(_window->device(), stagingBufferMemory);

    VulkanUtil::createBuffer(_window, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, _window->deviceLocalMemoryIndex(), _buffer, _memory);
    VulkanUtil::copyBuffer(_window, stagingBuffer, _buffer, size);

    _deviceFunctions->vkDestroyBuffer(_window->device(), stagingBuffer, nullptr);
    _deviceFunctions->vkFreeMemory(_window->device(), stagingBufferMemory, nullptr);

    if (_haveBuffer) {
        _deviceFunctions->vkQueueWaitIdle(_window->graphicsQueue());
        _deviceFunctions->vkDestroyBuffer(_window->device(), oldBuffer, nullptr);
        _deviceFunctions->vkFreeMemory(_window->device(), oldMemory, nullptr);
    }

    _haveBuffer = true;
}

