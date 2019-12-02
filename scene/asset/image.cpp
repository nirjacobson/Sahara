#include "image.h"
#include <QtDebug>
Sahara::Image::Image(const QString& uri)
    : _texture(QOpenGLTexture(QImage(uri)))
{

}

Sahara::Image::~Image()
{
    _texture.destroy();
}

void Sahara::Image::bind()
{
    _texture.bind();
}

void Sahara::Image::release()
{
    _texture.release();
}
