#ifndef SAHARA_MATERIAL_H
#define SAHARA_MATERIAL_H

#include <QColor>
#include <optional>

#include "material.h"
#include "vulkanimage.h"
#include "vulkanutil.h"
#include "../../render/vulkanwithuniform.h"

namespace Sahara
{

    class VulkanMaterial : public Material, public VulkanWithUniform
    {
        friend class JSON;

        public:
            VulkanMaterial(VulkanRenderer* renderer, const QString& id, const QString& name, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess);
            VulkanMaterial(VulkanRenderer* renderer, const QString& id, const QString& name, const QColor& emission, const QColor& ambient, VulkanImage* const image, const QColor& specular, const float shininess);
            ~VulkanMaterial();

            const QColor& emission() const;
            void setEmission(const QColor& color);
            const QColor& ambient() const;
            void setAmbient(const QColor& color);
            const QColor& diffuse() const;
            void setDiffuse(const QColor& color);
            const QColor& specular() const;
            void setSpecular(const QColor& color);
            float shininess() const;
            void setShininess(const float shininess);

            const QString& name() const;

            const std::optional<VulkanImage*>& image() const;
            std::optional<VulkanImage*>& image();

            const QList<VkDescriptorSet>& descriptorSets() const;

            void updateUniform(const uint32_t currentFrame);
            void recreateUniform();

        private:
            VulkanRenderer* _renderer;
            std::optional<VulkanImage*> _image;

            VulkanUtil::UniformBuffers _uniformBuffers;
    };

}

#endif // SAHARA_MATERIAL_H
