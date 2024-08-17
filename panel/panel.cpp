#include "panel.h"
#include "render/vulkanrenderer.h"

Sahara::Panel::Panel(VulkanRenderer *renderer, int width, int height)
    : _renderer(renderer)
    , _size(width, height)
    , _position(0, 0)
    , _image(width, height, QImage::Format_RGBA8888)
{
    generateVertexBuffers();

    renderer->createImage(_image.width(), _image.height(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, _vkImage, _vkImageMemory);
    _renderer->copyImage(_image, _vkImage);

    _vkImageView = renderer->createImageView(_vkImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

    _descriptorSets = renderer->createImageDescriptorSets(_vkImageView);
}

Sahara::Panel::~Panel()
{
    _renderer->freePanelImageDescriptorSets(_descriptorSets);
    _renderer->destroyImage(_vkImage, _vkImageMemory, _vkImageView);
}

const QSize &Sahara::Panel::size() const
{
    return _size;
}

void Sahara::Panel::setPosition(int x, int y)
{
    _position = QPoint(x, y);
}

const QPoint &Sahara::Panel::position()
{
    return _position;
}

void Sahara::Panel::generateVertexBuffers()
{
    float vertices[] = {
        0.0f,                 0.0f,
        0.0f,                 (float)_size.height(),
        (float)_size.width(), 0.0f,
        (float)_size.width(), (float)_size.height()
    };

    VulkanVertexBuffer* vertexBuffer = new VulkanVertexBuffer(_renderer->window());
    vertexBuffer->write(vertices, sizeof(vertices), 2);
    addVertexBuffer("position", vertexBuffer);

    float texcoords[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    vertexBuffer = new VulkanVertexBuffer(_renderer->window());
    vertexBuffer->write(texcoords, sizeof(texcoords), 2);
    addVertexBuffer("texcoord", vertexBuffer);
}

const QList<VkDescriptorSet> &Sahara::Panel::descriptorSets() const
{
    return _descriptorSets;
}

void Sahara::Panel::updateUniform(const uint32_t currentFrame)
{
    QPainter painter(&_image);

    if (update(painter)) {
        _renderer->copyImage(_image, _vkImage);
    }
}
