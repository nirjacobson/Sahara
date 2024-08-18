#ifndef SAHARA_VULKAN_INSTANCECONTROLLER_H
#define SAHARA_VULKAN_INSTANCECONTROLLER_H

#include "instancecontroller.h"
#include "vulkanutil.h"
#include "../../render/vulkanwithuniform.h"

namespace Sahara
{
    class VulkanRenderer;
    class VulkanInstanceController : public InstanceController, public VulkanWithUniform
    {
        friend class JSON;

        public:
            VulkanInstanceController(VulkanRenderer *renderer, const Armature* armature, const MaterialDict& materials, const QMatrix4x4& transform, Controller* controller);
            ~VulkanInstanceController();

            const QList<VkDescriptorSet>& descriptorSets() const;

            void updateUniform(const uint32_t currentFrame);
            void recreateUniform();

        private:
            VulkanRenderer* _renderer;

            VulkanUtil::UniformBuffers _uniformBuffers;
    };

}

#endif // SAHARA_VULKAN_INSTANCECONTROLLER_H
