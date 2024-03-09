#ifndef SAHARA_MATERIAL_H
#define SAHARA_MATERIAL_H

#include <QColor>
#include <optional>

#include "asset.h"
#include "image.h"
#include "vulkanutil.h"
#include "../../render/withuniform.h"

namespace Sahara
{

    class Material : public Asset, public WithUniform
    {
        friend class JSON;

        public:
            Material(Renderer* renderer, const QString& id, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess);
            Material(Renderer* renderer, const QString& id, const QColor& emission, const QColor& ambient, Image* const image, const QColor& specular, const float shininess);
            ~Material();

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

            const std::optional<Image*>& image() const;
            std::optional<Image*>& image();

            const QList<VkDescriptorSet>& descriptorSets() const;

            void updateUniform(const uint32_t currentFrame);

        private:
            Renderer* _renderer;
            QColor _emission;
            QColor _ambient;
            QColor _diffuse;
            std::optional<Image*> _image;
            QColor _specular;
            float _shininess;

            VulkanUtil::UniformBuffers _uniformBuffers;
    };

}

#endif // SAHARA_MATERIAL_H
