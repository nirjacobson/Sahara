#ifndef SAHARA_VULKAN_MODEL_H
#define SAHARA_VULKAN_MODEL_H

#include <QFileInfo>
#include <QDir>
#include <QStack>
#include <QMatrix4x4>

#include "scene/asset/mesh/vulkanmesh.h"
#include "scene/asset/controller.h"
#include "scene/asset/vulkanmaterial.h"
#include "scene/asset/armature.h"
#include "scene/asset/animation.h"
#include "scene/asset/animationclip.h"
#include "scene/instance/instance.h"
#include "scene/model.h"
#include "scene/asset/vulkanimage.h"
#include "collada/collada.h"
#include "common/volume.h"
#include "scene/instance/vulkaninstancecontroller.h"

namespace Sahara {

  class VulkanModel : public Model
  {
    friend class JSON;

    public:
      static VulkanModel* fromCollada(VulkanRenderer* renderer, const QString& path);

      void recreateUniforms();
    private:
      VulkanModel();

      static ImageDict parseColladaModelImages(VulkanRenderer* renderer, const QCollada::Collada& collada, const QString& path);
      static MaterialDict parseColladaModelMaterials(VulkanRenderer* renderer, const QCollada::Collada& collada, const ImageDict& images);
      static MeshDict parseColladaModelGeometries(QVulkanWindow* window, const QCollada::Collada& collada, Volume& volume);
      static ControllerDict parseColladaModelControllers(const QCollada::Collada& collada, MeshDict& meshes);
      static QList<Instance*> parseColladaVisualScene(VulkanRenderer* renderer, const QCollada::Collada& collada, const MaterialDict& materials, const MeshDict& meshes, const ControllerDict& controllers, Armature** const armaturePtr);
      static Armature* parseColladaArmatureNode(const QCollada::Node& rootNode);
      static Joint* parseColladaJointNode(const QCollada::Node& jointNode);
      static AnimationDict parseColladaModelAnimations(const QCollada::Collada& collada, Armature& armature);
      static AnimationClipDict parseColladaModelAnimationClips(const QCollada::Collada& collada, const AnimationDict& animations);
      static Animation* parseColladaModelAnimation(const QString& id, const QCollada::Animation& animation, Sahara::Armature& armature);
  };
}

#endif // SAHARA_VULKAN_MODEL_H
