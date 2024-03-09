#include "material.h"
#include "render/renderer.h"

Sahara::Material::Material(Renderer* renderer, const QString& id, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess)
    : Asset(id)
    , _renderer(renderer)
    , _emission(emission)
    , _ambient(ambient)
    , _diffuse(diffuse)
    , _specular(specular)
    , _shininess(shininess)
{
    _uniformBuffers = renderer->createMaterialUniformBuffers();
}

Sahara::Material::Material(Renderer* renderer, const QString& id, const QColor& emission, const QColor& ambient, Sahara::Image* const image, const QColor& specular, const float shininess)
    : Asset(id)
    , _renderer(renderer)
    , _emission(emission)
    , _ambient(ambient)
    , _image(image)
    , _specular(specular)
    , _shininess(shininess)
{
    _uniformBuffers = renderer->createMaterialUniformBuffers();
}

Sahara::Material::~Material()
{
    _renderer->destroyMaterialUniformBuffers(_uniformBuffers);
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

const std::optional<Sahara::Image*>& Sahara::Material::image() const
{
    return _image;
}

std::optional<Sahara::Image*>& Sahara::Material::image()
{
    return _image;
}

const QList<VkDescriptorSet>& Sahara::Material::descriptorSets() const
{
    return _uniformBuffers.bufferDescriptorSets;
}

void Sahara::Material::updateUniform(const uint32_t currentFrame) const
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
