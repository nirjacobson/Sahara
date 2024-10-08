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
    case Sahara::Surface::Input::POSITION:
        return "POSITION";
    case Sahara::Surface::Input::NORMAL:
        return "NORMAL";
    case Sahara::Surface::Input::TEXCOORD:
        return "TEXCOORD";
    case Sahara::Surface::Input::COLOR:
        return "COLOR";
    case Sahara::Surface::Input::JOINTS:
        return "JOINTS";
    case Sahara::Surface::Input::WEIGHTS:
        return "WEIGHTS";
    case Sahara::Surface::Input::UNDEFINED:
        return "UNDEFINED";
        break;
    }

    return "";
}

Sahara::Surface::Input::Semantic Sahara::Surface::Input::semanticFromString(const QString& semantic)
{
    if (semantic == "POSITION")
        return Sahara::Surface::Input::POSITION;
    if (semantic == "NORMAL")
        return Sahara::Surface::Input::NORMAL;
    if (semantic == "TEXCOORD")
        return Sahara::Surface::Input::TEXCOORD;
    if (semantic == "COLOR")
        return Sahara::Surface::Input::COLOR;
    if (semantic == "JOINTS")
        return Sahara::Surface::Input::JOINTS;
    if (semantic == "WEIGHTS")
        return Sahara::Surface::Input::WEIGHTS;

    return Sahara::Surface::Input::UNDEFINED;
}

Sahara::Surface::Surface(const SourceDict& sourceDict, const QString& material)
    : _sources(sourceDict)
    , _material(material)
{

}

int Sahara::Surface::count() const
{
    return _sources.first()->count();
}

const QString& Sahara::Surface::material() const
{
    return _material;
}

QList<Sahara::Surface::Input::Semantic> Sahara::Surface::inputs() const
{
    return _inputs.keys();
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

int Sahara::Surface::triangles() const
{
    int maxOffset = 0;
    for (Input i : _inputs) {
        if (i.offset() > maxOffset) {
            maxOffset = i.offset();
        }
    }
    return _elements.size() / (maxOffset + 1) / 3;
}
