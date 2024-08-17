#include "openglimage.h"
#include <QUrl>
Sahara::OpenGLImage::OpenGLImage(const QString& id, const QString& uri)
    : Image(id, uri)
    , _texture(QOpenGLTexture(QImage(QUrl::fromPercentEncoding(uri.toLatin1())).mirrored()))
{

}

Sahara::OpenGLImage::~OpenGLImage()
{
    _texture.destroy();
}

void Sahara::OpenGLImage::bind()
{
    _texture.bind();
}

void Sahara::OpenGLImage::release()
{
    _texture.release();
}

