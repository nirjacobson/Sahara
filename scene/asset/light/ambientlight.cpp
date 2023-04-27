#include "ambientlight.h"


Sahara::AmbientLight::AmbientLight()
    : Light(QColor(255, 255, 255))
    , _strength(0.2)
{

}

float Sahara::AmbientLight::strength() const
{
    return _strength;
}

void Sahara::AmbientLight::setStrength(const float strength)
{
    _strength = strength;
}
