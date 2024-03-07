#include "withvertexbuffers.h"


Sahara::WithVertexBuffers::WithVertexBuffers()
{

}

Sahara::WithVertexBuffers::~WithVertexBuffers()
{
    for (VertexBuffer* vb : _vertexBuffers.values()) {
        delete vb;
    }
}

const Sahara::VertexBufferDict &Sahara::WithVertexBuffers::vertexBuffers() const
{
    return _vertexBuffers;
}

Sahara::VertexBufferDict &Sahara::WithVertexBuffers::vertexBuffers()
{
    return _vertexBuffers;
}

QList<VkBuffer> Sahara::WithVertexBuffers::buffersByBinding(const Pipeline &pipeline)
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

void Sahara::WithVertexBuffers::addVertexBuffer(const QString &name, VertexBuffer* vertexBuffer)
{
    _vertexBuffers.insert(name, vertexBuffer);
}
