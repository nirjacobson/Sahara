#include "openglsurface.h"

Sahara::OpenGLSurface::OpenGLSurface(const SourceDict& sourceDict, const QString& material)
    : Surface(sourceDict, material)
{

}

void Sahara::OpenGLSurface::generateVertexBuffer(const Sahara::OpenGLSurface::Input::Semantic input)
{
    const Source* source = _sources[_inputs[input].source()];

    int maxOffset = 0;
    for (Input i : _inputs) {
        if (i.offset() > maxOffset) {
            maxOffset = i.offset();
        }
    }

    unsigned long dataSize = _elements.size() / (maxOffset + 1) * source->stride();
    GLfloat* data = new GLfloat[dataSize];
    int dataIndex = 0;

    for (int i = _inputs[input].offset(); i < _elements.size(); i += maxOffset + 1) {
        int index = _elements.at(i);
        QList<float> element = source->at(index);
        for (int j = 0; j < element.size(); j++) {
            data[dataIndex++] = element.at(j);
        }
    }

    OpenGLVertexBuffer vertexBuffer;
    vertexBuffer.write(data, dataSize);
    vertexBuffer.setStride(source->stride());

    delete [] data;

    OpenGLWithVertexBuffers::addVertexBuffer(Input::semanticToString(input).toLower(), vertexBuffer);
}
