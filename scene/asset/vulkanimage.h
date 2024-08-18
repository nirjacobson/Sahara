#ifndef SAHARA_VULKAN_IMAGE_H
#define SAHARA_VULKAN_IMAGE_H

#include <QImage>

#include "image.h"
#include "../../render/vulkanwithuniform.h"

namespace Sahara
{
    class VulkanRenderer;

    class VulkanImage : public Image, public VulkanWithUniform
    {
        friend class JSON;

        public:
            VulkanImage(VulkanRenderer* renderer, const QString& id, const QString& uri);
            ~VulkanImage();

            const QList<VkDescriptorSet>& descriptorSets() const;

            void updateUniform(const uint32_t currentFrame) { }
            void recreateUniform();

        private:
            VulkanRenderer* _renderer;
            VkImage _image;
            VkDeviceMemory _imageMemory;
            VkImageView _imageView;
            QList<VkDescriptorSet> _descriptorSets;
    };

}

#endif // SAHARA_VULKAN_IMAGE_H
