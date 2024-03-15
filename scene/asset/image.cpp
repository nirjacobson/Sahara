#include "image.h"
#include <QUrl>
Sahara::Image::Image(Renderer*, const QString& id, const QString& uri)
    : Asset(id)
    , _uri(uri)
    , _texture(QOpenGLTexture(QImage(QUrl::fromPercentEncoding(uri.toLatin1())).mirrored()))
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

const QString&Sahara::Image::uri() const
{
    return _uri;
}
