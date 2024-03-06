#ifndef SAHARA_INSTANCECONTROLLER_H
#define SAHARA_INSTANCECONTROLLER_H

#include "instance.h"
#include "../asset/controller.h"
#include "../asset/armature.h"
#include "vulkanutil.h"
#include "../../render/withuniform.h"

namespace Sahara
{
    class Renderer;
    class InstanceController : public Instance, public WithUniform
    {
        friend class JSON;

        public:
            InstanceController(Renderer *renderer, const Armature* armature, const MaterialDict& materials, const QMatrix4x4& transform, Controller* controller);
            ~InstanceController();

            Controller& controller();
            const Controller& controller() const;

            const Armature& armature() const;

            const QList<VkDescriptorSet>& descriptorSets() const;

            void updateUniform() const;

        private:
            Renderer* _renderer;
            const Armature* _armature;
            Controller* _controller;

            VulkanUtil::UniformBuffers _uniformBuffers;
    };

}

#endif // SAHARA_INSTANCECONTROLLER_H
