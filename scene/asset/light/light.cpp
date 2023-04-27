#include "light.h"


Sahara::Light::Light(const QColor& color)
    : _color(color)
{

}

const QColor& Sahara::Light::color() const
{
    return _color;
}

void Sahara::Light::setColor(const QColor& color) {
    _color = color;
}
