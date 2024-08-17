#ifndef SAHARA_VULKAN_IMAGE_H
#define SAHARA_VULKAN_IMAGE_H

#include <QImage>

#include "image.h"
#include "vulkanutil.h"
#include "../../render/withuniform.h"

namespace Sahara
{
    class VulkanRenderer;

    class VulkanImage : public Image, public WithUniform
    {
        friend class JSON;

        public:
            VulkanImage(VulkanRenderer* renderer, const QString& id, const QString& uri);
            ~VulkanImage();

            const QList<VkDescriptorSet>& descriptorSets() const;

            void updateUniform(const uint32_t currentFrame) { }

        private:
            VulkanRenderer* _renderer;
            VkImage _image;
            VkDeviceMemory _imageMemory;
            VkImageView _imageView;
            QList<VkDescriptorSet> _descriptorSets;
    };

}

#endif // SAHARA_VULKAN_IMAGE_H
