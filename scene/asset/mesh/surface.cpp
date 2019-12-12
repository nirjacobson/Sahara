#include "surface.h"

Sahara::Surface::Input::Input()
{

}

Sahara::Surface::Input::Input(const QString& source, const int offset)
    : _source(source)
    , _offset(offset)
{

}

const QString&Sahara::Surface::Input::source() const
{
    return _source;
}

int Sahara::Surface::Input::offset() const
{
    return _offset;
}

QString Sahara::Surface::Input::semanticToString(const Sahara::Surface::Input::Semantic semantic)
{
    switch (semantic) {
        case Sahara::Surface::Input::VERTEX:
            return "VERTEX";
        case Sahara::Surface::Input::NORMAL:
            return "NORMAL";
        case Sahara::Surface::Input::TEXCOORD:
            return "TEXCOORD";
        case Sahara::Surface::Input::COLOR:
            return "COLOR";
        case Sahara::Surface::Input::BONES:
            return "BONES";
        case Sahara::Surface::Input::WEIGHTS:
            return "WEIGHTS";
    }

    return "";
}

Sahara::Surface::Surface(Sahara::Mesh& mesh, const QString& material)
    : _mesh(mesh)
    , _material(material)
{

}

const QString&Sahara::Surface::material() const
{
    return _material;
}

QList<Sahara::Surface::Input::Semantic> Sahara::Surface::inputs() const
{
    return _inputs.keys();
}

QString Sahara::Surface::source(const Sahara::Surface::Input::Semantic semantic) const
{
    return _inputs[semantic].source();
}

int Sahara::Surface::offset(const Sahara::Surface::Input::Semantic semantic) const
{
    return _inputs[semantic].offset();
}

void Sahara::Surface::setInput(const Sahara::Surface::Input::Semantic semantic, const QString& source, const int offset)
{
    _inputs[semantic] = Input(source, offset);
}

const QList<int>&Sahara::Surface::elements() const
{
    return _elements;
}

void Sahara::Surface::setElements(const QList<int>& elements)
{
    _elements = elements;
}

void Sahara::Surface::generateVertexBuffer(const Sahara::Surface::Input::Semantic input)
{
    const Source& source = _mesh.source(_inputs[input].source());

    int dataSize = _elements.size() / _inputs.size() * source.stride();
    GLfloat* data = new GLfloat[static_cast<unsigned long>(dataSize)];
    int dataIndex = 0;

    for (int i = _inputs[input].offset(); i < _elements.size(); i += _inputs.size()) {
        int index = _elements.at(i);
        QList<float> element = source.at(index);
        for (int j = 0; j < element.size(); j++) {
            data[dataIndex++] = element.at(j);
        }
    }

    VertexBuffer vertexBuffer;
    vertexBuffer.write(data, dataSize);
    vertexBuffer.setStride(source.stride());

    WithVertexBuffers::addVertexBuffer(Input::semanticToString(input).toLower(), vertexBuffer);
}
