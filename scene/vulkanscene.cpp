#include "vulkanscene.h"
#include "render/vulkanrenderer.h"

Sahara::VulkanScene::VulkanScene(VulkanRenderer *renderer)
  : _renderer(renderer)
{
    _lightingUniformBuffers = renderer->createLightingUniformBuffers();
}

Sahara::VulkanScene::~VulkanScene()
{
  _renderer->destroyLightingUniformBuffers(_lightingUniformBuffers);
}

const QList<VkDescriptorSet>& Sahara::VulkanScene::descriptorSets() const
{
    return _lightingUniformBuffers.bufferDescriptorSets;
}

void Sahara::VulkanScene::updateUniform(const uint32_t currentFrame)
{
    ScenePipeline::Lighting lighting{
        .ambientLight = {
            .color = {
                _ambientLight.color().redF(),
                _ambientLight.color().greenF(),
                _ambientLight.color().blueF(),
            },
            .strength = _ambientLight.strength()
        },
        .pointLightCount = 0
    };

    _root->depthFirst([&](const Node& node) {
        if (!node.isRoot()) {
            const PointLight* pointLight;
            if ((pointLight = dynamic_cast<const PointLight*>(&node.item()))) {
                lighting.pointLights[lighting.pointLightCount++] = {
                    .position = {
                        node.globalPosition().x(),
                        node.globalPosition().y(),
                        node.globalPosition().z()
                    },
                    .color = {
                        pointLight->color().redF(),
                        pointLight->color().greenF(),
                        pointLight->color().blueF()
                    },
                    .attenuation = {
                        pointLight->constantAttenuation(),
                        pointLight->linearAttenuation(),
                        pointLight->quadraticAttenuation(),
                    }
                };
            }
        }
        return false;
    });

    memcpy(_lightingUniformBuffers.buffersMapped[currentFrame], &lighting, sizeof(ScenePipeline::Lighting));

}

void Sahara::VulkanScene::recreateUniform()
{
    _lightingUniformBuffers = _renderer->createLightingUniformBuffers();
}

void Sahara::VulkanScene::recreateUniforms()
{
    recreateUniform();

    _root->depthFirst([&](Sahara::Node& node) {
        if (!node.isRoot()) {
            VulkanModel* model;
            if ((model = dynamic_cast<VulkanModel*>(&node.item()))) {
                model->recreateUniforms();
            }
        }
        return false;
    });
}
