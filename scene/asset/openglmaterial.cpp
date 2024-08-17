#include "openglmaterial.h"

Sahara::OpenGLMaterial::OpenGLMaterial(const QString& id, const QString& name, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess)
    : Material(id, name, emission, ambient, diffuse, specular, shininess)
{

}

Sahara::OpenGLMaterial::OpenGLMaterial(const QString& id, const QString& name, const QColor& emission, const QColor& ambient, Sahara::OpenGLImage* const image, const QColor& specular, const float shininess)
    : Material(id, name, emission, ambient, QColor(), specular, shininess)
    , _image(image)
{

}

const std::optional<Sahara::OpenGLImage*>& Sahara::OpenGLMaterial::image() const
{
    return _image;
}

std::optional<Sahara::OpenGLImage*>& Sahara::OpenGLMaterial::image()
{
    return _image;
}
