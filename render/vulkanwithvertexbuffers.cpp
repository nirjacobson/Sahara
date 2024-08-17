#include "vulkanwithvertexbuffers.h"

Sahara::VulkanWithVertexBuffers::VulkanWithVertexBuffers()
{

}

Sahara::VulkanWithVertexBuffers::~VulkanWithVertexBuffers()
{
    for (VulkanVertexBuffer* vb : _vertexBuffers.values()) {
        delete vb;
    }
}

const Sahara::VulkanVertexBufferDict &Sahara::VulkanWithVertexBuffers::vertexBuffers() const
{
    return _vertexBuffers;
}

Sahara::VulkanVertexBufferDict &Sahara::VulkanWithVertexBuffers::vertexBuffers()
{
    return _vertexBuffers;
}

QList<VkBuffer> Sahara::VulkanWithVertexBuffers::buffersByBinding(const Pipeline &pipeline)
{
    QList<QString> vertexBufferNames = _vertexBuffers.keys();
    QList<VkBuffer> vertexBuffersSorted;
    std::sort(vertexBufferNames.begin(), vertexBufferNames.end(), [&](const QString& vb1, const QString& vb2) {
        return pipeline.binding(vb1) < pipeline.binding(vb2);
    });
    for (const QString& name : vertexBufferNames) {
        vertexBuffersSorted.append(_vertexBuffers[name]->buffer());
    }
    return vertexBuffersSorted;
}

void Sahara::VulkanWithVertexBuffers::addVertexBuffer(const QString &name, VulkanVertexBuffer* vertexBuffer)
{
    _vertexBuffers.insert(name, vertexBuffer);
}
