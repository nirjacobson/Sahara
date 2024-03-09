#include "image.h"
#include "render/renderer.h"
#include <QUrl>

Sahara::Image::Image(Renderer *renderer, const QString &id, const QString &uri)
    : Asset(id)
    , _renderer(renderer)
    , _uri(uri)
{
    QImage image = (uri.isEmpty()
                       ? QImage(1, 1, QImage::Format_RGBA8888)
                        : QImage(QUrl::fromPercentEncoding(uri.toLatin1())).mirrored())
                    .convertToFormat(QImage::Format::Format_RGBA8888);

    renderer->createImage(image.width(), image.height(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, _image, _imageMemory);
    renderer->copyImage(image, _image);

    _imageView = renderer->createImageView(_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

    _descriptorSets = renderer->createImageDescriptorSets(_imageView);
}

Sahara::Image::~Image()
{
    _renderer->freeImageDescriptorSets(_descriptorSets);
    _renderer->destroyImage(_image, _imageMemory, _imageView);
}

const QList<VkDescriptorSet>& Sahara::Image::descriptorSets() const
{
    return _descriptorSets;
}

const QString&Sahara::Image::uri() const
{
    return _uri;
}
