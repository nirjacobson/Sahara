#ifndef SAHARA_VULKAN_SCENE_H
#define SAHARA_VULKAN_SCENE_H

#include <functional>

#include "asset/camera.h"
#include "asset/light/light.h"
#include "asset/light/ambientlight.h"
#include "node/node.h"
#include "vulkanutil.h"
#include "../render/vulkanwithuniform.h"
#include "scene/vulkanmodel.h"
#include "scene.h"

namespace Sahara {
  class VulkanRenderer;

  class VulkanScene : public Scene, public VulkanWithUniform
  {
    friend class JSON;

    public:
      VulkanScene(VulkanRenderer* renderer);
      ~VulkanScene();

      const QList<VkDescriptorSet>& descriptorSets() const;

      void updateUniform(const uint32_t currentFrame);
      void recreateUniform();
      void recreateUniforms();

    private:
      VulkanRenderer* _renderer;

      VulkanUtil::UniformBuffers _lightingUniformBuffers;
  };

}

#endif // SAHARA_VULKAN_SCENE_H
