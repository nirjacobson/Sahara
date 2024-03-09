#ifndef WITHUNIFORM_H
#define WITHUNIFORM_H

#include <QList>
#include <QVulkanDeviceFunctions>

namespace Sahara {
    class WithUniform
    {
    public:
        virtual const QList<VkDescriptorSet>& descriptorSets() const = 0;

        virtual void updateUniform(const uint32_t currentFrame) = 0;
    private:

    };
}

#endif // WITHUNIFORM_H
