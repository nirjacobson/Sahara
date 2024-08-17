#include "vulkansurface.h"

Sahara::VulkanSurface::VulkanSurface(QVulkanWindow* window, const SourceDict& sourceDict, const QString& material)
    : Surface(sourceDict, material)
    , _vulkanWindow(window)
{

}

void Sahara::VulkanSurface::generateVertexBuffer(const Sahara::VulkanSurface::Input::Semantic input)
{
    const Source* source = _sources[_inputs[input].source()];

    int maxOffset = 0;
    for (Input i : _inputs) {
        if (i.offset() > maxOffset) {
            maxOffset = i.offset();
        }
    }

    unsigned long dataSize = _elements.size() / (maxOffset + 1) * source->stride();
    float* data = new float[dataSize];
    int dataIndex = 0;

    for (int i = _inputs[input].offset(); i < _elements.size(); i += maxOffset + 1) {
        int index = _elements.at(i);
        QList<float> element = source->at(index);
        for (int j = 0; j < element.size(); j++) {
            data[dataIndex++] = element.at(j);
        }
    }

    int stride;
    switch (input) {
    case Input::POSITION:
    case Input::NORMAL:
    case Input::COLOR:
        stride = 3;
        break;
    case Input::TEXCOORD:
        stride = 2;
        break;
    case Input::JOINTS:
    case Input::WEIGHTS:
        stride = 4;
        break;
    default:
    case Input::UNDEFINED:
        stride = 1;
        break;
    }

    VulkanVertexBuffer* vertexBuffer = new VulkanVertexBuffer(_vulkanWindow);
    vertexBuffer->write(data, dataSize * sizeof(float), stride);

    delete [] data;

    VulkanWithVertexBuffers::addVertexBuffer(Input::semanticToString(input).toLower(), vertexBuffer);
}
