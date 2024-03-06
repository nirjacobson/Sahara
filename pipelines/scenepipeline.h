#ifndef SCENEPIPELINE_H
#define SCENEPIPELINE_H

#include "pipeline.h"
#include "vulkanutil.h"

class ScenePipeline : public Pipeline
{
public:
    ~ScenePipeline();

    struct PushConstants {
        alignas(16) float modelView[16];
        alignas(16) int articulated;
        alignas(16) float cameraPosition[3];
        alignas(16) int focus;
    };

    struct Render {
        alignas(16) float inverseCamera[16];
        alignas(16) float projection[16];
    };

    struct Joint {
        float rotation[4];
        float translation[3];
    };

    struct Armature {
        alignas(16) Joint joints[110];
    };

    struct AmbientLight {
        float color[3];
        float strength;
    };

    struct PointLight {
        float position[3];
        float color[3];
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;
    };

    struct Lighting {
        alignas(16) AmbientLight ambientLight;
        alignas(16) PointLight pointLights[6];
        alignas(16) int pointLightCount;
    };

    struct Material {
        alignas(16) float emission[4];
        alignas(16) float ambient[4];
        alignas(16) float diffuse[4];
        alignas(16) float specular[4];
        alignas(16) float shininess;
        alignas(16) int textured;
    };

    ScenePipeline(QVulkanWindow* vulkanWindow, VkPolygonMode polygonMode);

    void create();
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

    QList<VkVertexInputBindingDescription> getVertexInputBindingDescriptions();
    QList<VkDescriptorSetLayoutBinding> getDescriptorSetLayoutBindings();
    QList<VkVertexInputAttributeDescription> getVertexInputAttributeDescriptions();
};

#endif // SCENEPIPELINE_H
