#include "material.h"

Sahara::Material::Material(const QVector4D& emission, const QVector4D& ambient, const QVector4D& diffuse, const QVector4D& specular, const float shininess)
    : _emission(emission)
    , _ambient(ambient)
    , _diffuse(diffuse)
    , _specular(specular)
    , _shininess(shininess)
{

}

Sahara::Material::Material(const QVector4D& emission, const QVector4D& ambient, Sahara::Image* const image, const QVector4D& specular, const float shininess)
    : _emission(emission)
    , _ambient(ambient)
    , _image(image)
    , _specular(specular)
    , _shininess(shininess)
{

}

const QVector4D& Sahara::Material::emission() const
{
    return _emission;
}

const QVector4D& Sahara::Material::ambient() const
{
    return _ambient;
}

const QVector4D& Sahara::Material::diffuse() const
{
    return _diffuse;
}

const QVector4D& Sahara::Material::specular() const
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
