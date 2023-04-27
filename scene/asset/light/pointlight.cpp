#include "pointlight.h"


Sahara::PointLight::PointLight(const QString& id, const QColor& color, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation)
    : Asset(id)
    , Light(color)
    , _constantAttenuation(constantAttenuation)
    , _linearAttenuation(linearAttenuation)
    , _quadraticAttenuation(quadraticAttenuation)
{

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

Sahara::Volume Sahara::PointLight::volume() const
{
    return {{-0.4755289f, -0.8258762f, -0.5f}, {0.4755289f, 0.5495058f, 0.5f}};
}
