#ifndef ANIMATEDPIPELINE_H
#define ANIMATEDPIPELINE_H

#include "pipeline.h"
#include "vulkanutil.h"

class AnimatedPipeline : public Pipeline
{
public:
    ~AnimatedPipeline();

    struct PushConstants {
        alignas(16) float modelView[16];
        alignas(16) int articulated;
        alignas(16) float cameraPosition[4];
        alignas(16) int focus;
    };

    struct Render {
        alignas(16) float inverseCamera[16];
        alignas(16) float projection[16];
    };

    struct Joint {
        alignas(16) float rotation[4];
        alignas(16) float translation[3];
    };

    struct Armature {
        Joint joints[110];
    };

    struct AmbientLight {
        float color[3];
        float strength;
    };

    struct PointLight {
        alignas(16) float position[3];
        alignas(16) float color[3];
        alignas(16) float attenuation[3];
    };

    struct Lighting {
        alignas(16) AmbientLight ambientLight;
        alignas(16) int pointLightCount;
        PointLight pointLights[6];
    };

    struct Material {
        alignas(16) float emission[4];
        alignas(16) float ambient[4];
        alignas(16) float diffuse[4];
        alignas(16) float specular[4];
        alignas(16) float shininess;
        alignas(16) int textured;
    };

    AnimatedPipeline(QVulkanWindow* vulkanWindow, VkPolygonMode polygonMode);

    void init();
    uint32_t binding(const QString &vertexAttribute) const;

    QList<VkDescriptorSet> createImageDescriptorSets(VkImageView imageView);

private:

    struct Vertex {
        float position[3];
        float normal[3];
        float texcoord[2];
        float joints[4];
        float weights[4];
    };

    QVulkanWindow* _vulkanWindow;

    VkSampler _sampler;

    QList<VkVertexInputBindingDescription> getVertexInputBindingDescriptions() const;
    QList<QList<VkDescriptorSetLayoutBinding>> getDescriptorSetLayoutBindings() const;
    QList<VkVertexInputAttributeDescription> getVertexInputAttributeDescriptions() const;
};

#endif // ANIMATEDPIPELINE_H
