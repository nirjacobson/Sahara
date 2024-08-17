#include "image.h"

Sahara::Image::Image(const QString& id, const QString& uri)
    : Asset(id)
    , _uri(uri)
{

}

const QString& Sahara::Image::uri() const
{
    return _uri;
}
