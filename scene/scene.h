#ifndef SAHARA_SCENE_H
#define SAHARA_SCENE_H

#include <functional>

#include "asset/camera.h"
#include "asset/light/light.h"
#include "asset/light/ambientlight.h"
#include "model.h"
#include "node/node.h"
#include "vulkanutil.h"
#include "../render/withuniform.h"

namespace Sahara {
  class Renderer;

  class Scene : public WithUniform
  {
    friend class JSON;

    public:
      Scene(Renderer* renderer);
      ~Scene();

      const Node& root() const;
      Node& root();

      const QMap<QString, Camera*>& cameras() const;
      const QMap<QString, Light*>& lights() const;
      const QMap<QString, Model*>& models() const;

      void addCamera(const QString& id, Camera* const camera);
      void addLight(const QString& id, Light* const light);
      void addModel(const QString& id, Model* const model);

      void setCameraNode(Sahara::Node* node);
      Node& cameraNode();
      const Node& cameraNode() const;
      Camera& camera();
      const Camera& camera() const;
      Node* focusNode();

      const AmbientLight& ambientLight() const;
      AmbientLight& ambientLight();

      const QList<VkDescriptorSet>& descriptorSets() const;

      void updateUniform(const uint32_t currentFrame) const;

    private:
      Renderer* _renderer;

      AmbientLight _ambientLight;

      Node* _root;

      Node* _cameraNode;

      QMap<QString, Camera*> _cameras;
      QMap<QString, Light*> _lights;
      QMap<QString, Model*> _models;

      VulkanUtil::UniformBuffers _lightingUniformBuffers;
  };

}

#endif // SAHARA_SCENE_H
