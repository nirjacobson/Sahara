#include "pointlight.h"


Sahara::PointLight::PointLight(const QString& id, const QColor& color, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation)
    : Light(id)
    , _color(color)
    , _constantAttenuation(constantAttenuation)
    , _linearAttenuation(linearAttenuation)
    , _quadraticAttenuation(quadraticAttenuation)
{

}

const QColor& Sahara::PointLight::color() const
{
    return _color;
}

float Sahara::PointLight::constantAttenuation() const
{
    return _constantAttenuation;
}

float Sahara::PointLight::linearAttenuation() const
{
    return _linearAttenuation;
}

float Sahara::PointLight::quadraticAttenuation() const
{
    return _quadraticAttenuation;
}

void Sahara::PointLight::setColor(const QColor& color)
{
    _color = color;
}

void Sahara::PointLight::setConstantAttenuation(const float constantAttenuation)
{
    _constantAttenuation = constantAttenuation;
}

void Sahara::PointLight::setLinearAttenuation(const float linearAttenuation)
{
    _linearAttenuation = linearAttenuation;
}

void Sahara::PointLight::setQuadraticAttenuation(const float quadraticAttenuation)
{
    _quadraticAttenuation = quadraticAttenuation;
}
