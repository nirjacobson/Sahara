#include "vulkanimage.h"
#include "render/vulkanrenderer.h"
#include <QUrl>

Sahara::VulkanImage::VulkanImage(VulkanRenderer *renderer, const QString &id, const QString &uri)
    : Image(id, uri)
    , _renderer(renderer)
{
    QImage image = (uri.isEmpty()
                       ? QImage(1, 1, QImage::Format_RGBA8888)
                        : QImage(QUrl::fromPercentEncoding(uri.toLatin1())).mirrored())
                    .convertToFormat(QImage::Format::Format_RGBA8888);

    _renderer->createImage(image.width(), image.height(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, _image, _imageMemory);
    _renderer->copyImage(image, _image);

    _imageView = _renderer->createImageView(_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

    _descriptorSets = _renderer->createImageDescriptorSets(_imageView);
}

Sahara::VulkanImage::~VulkanImage()
{
    _renderer->freeImageDescriptorSets(_descriptorSets);
    _renderer->destroyImage(_image, _imageMemory, _imageView);
}

const QList<VkDescriptorSet>& Sahara::VulkanImage::descriptorSets() const
{
    return _descriptorSets;
}
