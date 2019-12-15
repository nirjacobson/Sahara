#ifndef SAHARA_MODEL_H
#define SAHARA_MODEL_H

#include "asset/mesh/surface.h"
#include "asset/controller.h"
#include "asset/material.h"
#include "asset/armature.h"
#include "asset/animation.h"
#include "asset/animationclip.h"
#include "node/nodeitem.h"
#include "instance/instance.h"
#include "instance/instancecontroller.h"
#include "instance/instancemesh.h"
#include "asset/image.h"
#include "collada/collada.h"
#include "common/volume.h"
#include "common/transform.h"

namespace Sahara {

  typedef QMap<QString, Image*> ImageDict;
  typedef QMap<QString, Material*> MaterialDict;
  typedef QMap<QString, Mesh*> MeshDict;
  typedef QMap<QString, Controller*> ControllerDict;
  typedef QMap<QString, Animation*> AnimationDict;
  typedef QMap<QString, AnimationClip*> AnimationClipDict;

  class Model : public NodeItem
  {
    friend class JSON;

    private:
      Model();

    public:
      ~Model();

      Volume volume() const override;

      const ImageDict& images() const;
      const MaterialDict& materials() const;
      const MeshDict& meshes() const;
      const ControllerDict& controllers() const;
      const QList<Instance*> instances() const ;
      const AnimationDict& animations() const;
      const AnimationClipDict& animationClips() const;
      const Armature& armature() const;

      static Model* fromCollada(const QString& path);

      QStringList animationClipNames() const;
      void setAnimationClip(const QString& name);

      void animate(const float time);

    private:
      Volume _volume;
      ImageDict _images;
      MaterialDict _materials;
      MeshDict _meshes;
      ControllerDict _controllers;
      AnimationDict _animations;
      AnimationClipDict _animationClips;

      QList<Instance*> _instances;
      Armature* _armature;
      AnimationClip* _animationClip;

      static Model* parseColladaModel(const QString& path);
      static ImageDict parseColladaModelImages(const QCollada::Collada& collada, const QString& path);
      static MaterialDict parseColladaModelMaterials(const QCollada::Collada& collada, const ImageDict& images);
      static MeshDict parseColladaModelGeometries(const QCollada::Collada& collada, Volume& volume);
      static QList<QPair<int, float>> reduceBones(const QList<QPair<int, float>>& bones, const int max);
      static ControllerDict parseColladaModelControllers(const QCollada::Collada& collada, MeshDict& meshes);
      static QList<Instance*> parseColladaVisualScene(const QCollada::Collada& collada, const MaterialDict& materials, const MeshDict& meshes, const ControllerDict& controllers, Armature** const armaturePtr);
      static Armature* parseColladaArmatureNode(const QCollada::Node& rootNode);
      static Bone* parseColladaBoneNode(const QCollada::Node& boneNode);
      static AnimationDict parseColladaModelAnimations(const QCollada::Collada& collada, Armature& armature);
      static AnimationClipDict parseColladaModelAnimationClips(const QCollada::Collada& collada, const AnimationDict& animations);
      static Animation* parseColladaModelAnimation(const QString& id, const QCollada::Animation& animation, Sahara::Armature& armature);
  };
}

#endif // SAHARA_MODEL_H
