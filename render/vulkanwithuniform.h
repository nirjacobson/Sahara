#ifndef SAHARA_VULKANWITHUNIFORM_H
#define SAHARA_VULKANWITHUNIFORM_H

#include <QList>
#include <QVulkanDeviceFunctions>

namespace Sahara {
    class VulkanWithUniform
    {
    public:
        virtual const QList<VkDescriptorSet>& descriptorSets() const = 0;

        virtual void updateUniform(const uint32_t currentFrame) = 0;

        virtual void recreateUniform() = 0;
    private:

    };
}

#endif // SAHARA_VULKANWITHUNIFORM_H
