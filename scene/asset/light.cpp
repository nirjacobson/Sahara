#include "light.h"


Sahara::Light::Light(const QString& id)
    : Asset(id)
{

}

Sahara::Volume Sahara::Light::volume() const
{
    return {{-0.4755289f, -0.8258762f, -0.5f}, {0.4755289f, 0.5495058f, 0.5f}};
}
