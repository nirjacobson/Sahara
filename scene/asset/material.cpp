#include "material.h"

Sahara::Material::Material(const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess)
    : _emission(emission)
    , _ambient(ambient)
    , _diffuse(diffuse)
    , _specular(specular)
    , _shininess(shininess)
{

}

Sahara::Material::Material(const QColor& emission, const QColor& ambient, Sahara::Image* const image, const QColor& specular, const float shininess)
    : _emission(emission)
    , _ambient(ambient)
    , _image(image)
    , _specular(specular)
    , _shininess(shininess)
{

}

const QColor& Sahara::Material::emission() const
{
    return _emission;
}

const QColor& Sahara::Material::ambient() const
{
    return _ambient;
}

const QColor& Sahara::Material::diffuse() const
{
    return _diffuse;
}

const QColor& Sahara::Material::specular() const
{
    return _specular;
}

float Sahara::Material::shininess() const
{
    return _shininess;
}

const std::optional<Sahara::Image*>& Sahara::Material::image() const
{
    return _image;
}

std::optional<Sahara::Image*>& Sahara::Material::image()
{
    return _image;
}
