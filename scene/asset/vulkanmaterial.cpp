#include "vulkanmaterial.h"
#include "render/vulkanrenderer.h"

Sahara::VulkanMaterial::VulkanMaterial(VulkanRenderer* renderer, const QString& id, const QString& name,  const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess)
    : Material(id, name, emission, ambient, diffuse, specular, shininess)
    , _renderer(renderer)
{
    _uniformBuffers = renderer->createMaterialUniformBuffers();
}

Sahara::VulkanMaterial::VulkanMaterial(VulkanRenderer* renderer, const QString& id, const QString& name,  const QColor& emission, const QColor& ambient, Sahara::VulkanImage* const image, const QColor& specular, const float shininess)
    : Material(id, name, emission, ambient, QColor(), specular, shininess)
    , _renderer(renderer)
    , _image(image)
{
    _uniformBuffers = renderer->createMaterialUniformBuffers();
}

Sahara::VulkanMaterial::~VulkanMaterial()
{
    _renderer->destroyMaterialUniformBuffers(_uniformBuffers);
}

const std::optional<Sahara::VulkanImage*>& Sahara::VulkanMaterial::image() const
{
    return _image;
}

std::optional<Sahara::VulkanImage*>& Sahara::VulkanMaterial::image()
{
    return _image;
}

const QList<VkDescriptorSet>& Sahara::VulkanMaterial::descriptorSets() const
{
    return _uniformBuffers.bufferDescriptorSets;
}

void Sahara::VulkanMaterial::updateUniform(const uint32_t currentFrame)
{
    ScenePipeline::Material material{
        .emission = {
            _emission.redF(),
            _emission.greenF(),
            _emission.blueF(),
            _emission.alphaF()
        },
        .ambient = {
            _ambient.redF(),
            _ambient.greenF(),
            _ambient.blueF(),
            _ambient.alphaF()
        },
        .diffuse = {
            _diffuse.redF(),
            _diffuse.greenF(),
            _diffuse.blueF(),
            _diffuse.alphaF()
        },
        .specular = {
            _specular.redF(),
            _specular.greenF(),
            _specular.blueF(),
            _specular.alphaF()
        },
        .shininess = _shininess,
        .textured = _image.has_value()
    };

    memcpy(_uniformBuffers.buffersMapped[currentFrame], &material, sizeof(ScenePipeline::Material));
}
