#include "material.h"

Sahara::Material::Material(const QString& id, const QString& name, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess)
    : Asset(id)
    , _name(name)
    , _emission(emission)
    , _ambient(ambient)
    , _diffuse(diffuse)
    , _specular(specular)
    , _shininess(shininess)
{

}

Sahara::Material::Material(const QString& id, const QString& name, const QColor& emission, const QColor& ambient, Sahara::Image* const image, const QColor& specular, const float shininess)
    : Asset(id)
    , _name(name)
    , _emission(emission)
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

void Sahara::Material::setEmission(const QColor& color)
{
    _emission = color;
}

const QColor& Sahara::Material::ambient() const
{
    return _ambient;
}

void Sahara::Material::setAmbient(const QColor& color)
{
    _ambient = color;
}

const QColor& Sahara::Material::diffuse() const
{
    return _diffuse;
}

void Sahara::Material::setDiffuse(const QColor& color)
{
    _diffuse = color;
}

const QColor& Sahara::Material::specular() const
{
    return _specular;
}

void Sahara::Material::setSpecular(const QColor& color)
{
    _specular = color;
}

float Sahara::Material::shininess() const
{
    return _shininess;
}

void Sahara::Material::setShininess(const float shininess)
{
    _shininess = shininess;
}

const QString& Sahara::Material::name() const
{
    return _name;
}

const std::optional<Sahara::Image*>& Sahara::Material::image() const
{
    return _image;
}

std::optional<Sahara::Image*>& Sahara::Material::image()
{
    return _image;
}
