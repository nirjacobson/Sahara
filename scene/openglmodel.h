#ifndef SAHARA_OPENGL_MODEL_H
#define SAHARA_OPENGL_MODEL_H

#include "Sahara_global.h"

#include <QStack>
#include <QMatrix4x4>

#include "model.h"
#include "scene/asset/openglmaterial.h"
#include "collada/collada.h"

namespace Sahara {
  class OpenGLRenderer;

  class SAHARA_EXPORT OpenGLModel : public Model
  {
    friend class JSON;

    public:
      static OpenGLModel* fromCollada(const QString& path);

    private:
      OpenGLModel();

      static ImageDict parseColladaModelImages(const QCollada::Collada& collada, const QString& path);
      static MaterialDict parseColladaModelMaterials(const QCollada::Collada& collada, const ImageDict& images);
      static MeshDict parseColladaModelGeometries(const QCollada::Collada& collada, Volume& volume);
      static ControllerDict parseColladaModelControllers(const QCollada::Collada& collada, MeshDict& meshes);
      static QList<Instance*> parseColladaVisualScene(const QCollada::Collada& collada, const MaterialDict& materials, const MeshDict& meshes, const ControllerDict& controllers, Armature** const armaturePtr);
      static Armature* parseColladaArmatureNode(const QCollada::Node& rootNode);
      static Joint* parseColladaJointNode(const QCollada::Node& jointNode);
      static AnimationDict parseColladaModelAnimations(const QCollada::Collada& collada, Armature& armature);
      static AnimationClipDict parseColladaModelAnimationClips(const QCollada::Collada& collada, const AnimationDict& animations);
      static Animation* parseColladaModelAnimation(const QString& id, const QCollada::Animation& animation, Sahara::Armature& armature);
  };
}

#endif // SAHARA_OPENGL_MODEL_H
