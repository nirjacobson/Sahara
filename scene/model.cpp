#include "model.h"

#include "instance/instancemesh.h"
#include "instance/instancecontroller.h"

#include "../QCollada/instance/instancecontroller.h"
#include "../QCollada/instance/instancegeometry.h"
#include "../QCollada/asset/common/floatsource.h"
#include "../QCollada/asset/common/namesource.h"

Sahara::Model::Model()
{

}

Sahara::Model::~Model()
{
  for (Sahara::Image* image : _images.values()) {
    delete image;
  }
  for (Sahara::Material* material : _materials.values()) {
    delete material;
  }
  for (Sahara::Mesh* mesh : _meshes.values()) {
    delete mesh;
  }
  delete _armature;
  for (Sahara::Instance* instance : _instances) {
    delete instance;
  }
  for (Sahara::AnimationClip* animationClip : _animationClips.values()) {
    delete animationClip;
  }
}

Sahara::Volume Sahara::Model::volume() const
{
  return _volume;
}

const Sahara::ImageDict& Sahara::Model::images() const
{
  return _images;
}

const Sahara::MaterialDict& Sahara::Model::materials() const
{
  return _materials;
}

const Sahara::MeshDict& Sahara::Model::meshes() const
{
  return _meshes;
}

const Sahara::ControllerDict& Sahara::Model::controllers() const
{
  return _controllers;
}

const QList<Sahara::Instance*> Sahara::Model::instances() const {
    return _instances;
}

const Sahara::AnimationDict&Sahara::Model::animations() const
{
    return _animations;
}

const Sahara::AnimationClipDict& Sahara::Model::animationClips() const
{
  return _animationClips;
}

const Sahara::Armature& Sahara::Model::armature() const
{
  return *_armature;
}

Sahara::Model* Sahara::Model::fromCollada(const QString& path)
{
  return parseColladaModel(path);
}

QStringList Sahara::Model::animationClipNames() const
{
  QStringList names;

  for (auto it = _animationClips.begin(); it != _animationClips.end(); it++)
    names.append(it.value()->name());

  return names;
}

void Sahara::Model::setAnimationClip(const QString& name)
{
  for (auto it = _animationClips.begin(); it != _animationClips.end(); it++) {
    if (it.value()->name() == name) {
      _animationClip = it.value();
      return;
    }
  }
}

QString Sahara::Model::animationClip() const
{
    return _animationClip ? _animationClip->name() : "";
}

int Sahara::Model::triangles() const
{
    int triangles = 0;
    for (Sahara::Mesh* mesh : _meshes.values()) {
      triangles += mesh->triangles();
    }

    return triangles;
}

void Sahara::Model::animate(const float time)
{
  if (_animationClip != nullptr) {
    _animationClip->apply(time);
  }
}

Sahara::Model* Sahara::Model::parseColladaModel(const QString& path)
{
  QCollada::Collada collada = QCollada::Collada::parse(path);
  Sahara::Model* model = new Model();

  ImageDict images = parseColladaModelImages(collada, QFileInfo(path).dir().path());
  MaterialDict materials = parseColladaModelMaterials(collada, images);
  MeshDict meshes = parseColladaModelGeometries(collada, model->_volume);
  ControllerDict controllers = parseColladaModelControllers(collada, meshes);
  Armature* armature;
  QList<Sahara::Instance*> instances = parseColladaVisualScene(collada, materials, meshes, controllers, &armature);
  Sahara::AnimationDict animations = parseColladaModelAnimations(collada, *armature);
  Sahara::AnimationClipDict animationClips = parseColladaModelAnimationClips(collada, animations);

  model->_images = images;
  model->_materials = materials;
  model->_meshes = meshes;
  model->_controllers = controllers;
  model->_armature = armature;
  model->_instances = instances;
  model->_animations = animations;
  model->_animationClips = animationClips;
  model->_animationClip = animationClips.isEmpty() ? nullptr : animationClips.first();

  return model;
}

Sahara::ImageDict Sahara::Model::parseColladaModelImages(const QCollada::Collada& collada, const QString& path)
{
  Sahara::ImageDict images;

  for (auto it = collada.images().begin(); it != collada.images().end(); it++) {
    QString id = it.key();
    QCollada::Image* image = it.value();
    Sahara::Image* modelImage = new Sahara::Image(id, QDir::cleanPath(path + QDir::separator() + image->initFrom()));

    images.insert(id, modelImage);
  }

  return images;
}

Sahara::MaterialDict Sahara::Model::parseColladaModelMaterials(const QCollada::Collada& collada, const Sahara::ImageDict& images)
{
  Sahara::MaterialDict materials;

  for (auto it = collada.materials().begin(); it != collada.materials().end(); it++) {
    QString id = it.key();
    QCollada::Material* material = it.value();
    const QCollada::Asset* effectAsset = collada.resolve(material->instanceEffect().url());
    const QCollada::Effect* effect;
    if ((effect = dynamic_cast<const QCollada::Effect*>(effectAsset))) {
      Sahara::Material* modelMaterial;
      if (effect->sampler2D().has_value()) {
        QString imageId = effect->sampler2D()->source();
        Sahara::Image* image = images[imageId];
        modelMaterial = new Sahara::Material(
          id,
          effect->phong().emission(),
          effect->phong().ambient(),
          image,
          effect->phong().specular(),
          effect->phong().shininess());
      } else {
        modelMaterial = new Sahara::Material(
          id,
          effect->phong().emission(),
          effect->phong().ambient(),
          effect->phong().diffuse(),
          effect->phong().specular(),
          effect->phong().shininess());
      }

      materials.insert(id, modelMaterial);
    }
  }

  return materials;
}

Sahara::MeshDict Sahara::Model::parseColladaModelGeometries(const QCollada::Collada& collada, Volume& volume)
{
  MeshDict meshes;

  QVector3D lowerVertex;
  QVector3D upperVertex;

  for (auto it = collada.geometries().begin(); it != collada.geometries().end(); it++) {
    QString id = it.key();
    QCollada::Geometry* geometry = it.value();
    Sahara::Mesh* modelMesh = new Sahara::Mesh(id);

    for (const QCollada::Triangles& triangles : geometry->mesh().triangles()) {
      Surface& meshSurface = modelMesh->add(triangles.material());
      for (auto it = triangles.inputs().begin(); it != triangles.inputs().end(); ++it) {
        QCollada::Triangles::Semantic semantic = it.key();
        QString url = it.value().first;
        QString sourceName = url.mid(1);
        int offset = it.value().second;

        if (!modelMesh->sources().contains(sourceName)) {
            const QCollada::Source& source = (semantic == QCollada::Triangles::Semantic::VERTEX)
              ? geometry->mesh().getSource(geometry->mesh().vertices().inputs().begin().value())
              : geometry->mesh().getSource(url);
            const QCollada::FloatSource& floatSource = dynamic_cast<const QCollada::FloatSource&>(source);

            Sahara::Source* meshSource = new Sahara::Source(floatSource.data(), floatSource.accessor().stride());
            modelMesh->add(sourceName, meshSource);

            if (semantic == QCollada::Triangles::Semantic::VERTEX) {
                for (int i = 0; i < floatSource.accessor().count(); i++) {
                    GLfloat x = floatSource.data().at(i * floatSource.accessor().stride() + 0);
                    GLfloat y = floatSource.data().at(i * floatSource.accessor().stride() + 1);
                    GLfloat z = floatSource.data().at(i * floatSource.accessor().stride() + 2);

                    if (x < lowerVertex.x())
                        lowerVertex.setX(x);
                    if (y < lowerVertex.y())
                        lowerVertex.setY(y);
                    if (z < lowerVertex.z())
                        lowerVertex.setZ(z);

                    if (x > upperVertex.x())
                        upperVertex.setX(x);
                    if (y > upperVertex.y())
                        upperVertex.setY(y);
                    if (z > upperVertex.z())
                        upperVertex.setZ(z);
                }
            }
        }

        Sahara::Surface::Input::Semantic surfaceSemantic;
        switch (semantic) {
            case QCollada::Triangles::VERTEX:
                surfaceSemantic = Sahara::Surface::Input::Semantic::POSITION;
                break;
            case QCollada::Triangles::NORMAL:
                surfaceSemantic = Sahara::Surface::Input::Semantic::NORMAL;
                break;
            case QCollada::Triangles::TEXCOORD:
                surfaceSemantic = Sahara::Surface::Input::Semantic::TEXCOORD;
                break;
            case QCollada::Triangles::COLOR:
                surfaceSemantic = Sahara::Surface::Input::Semantic::COLOR;
                break;

        }
        meshSurface.setInput(surfaceSemantic, sourceName, offset);
      }
      meshSurface.setElements(triangles.p());

      for (const Sahara::Surface::Input::Semantic input : meshSurface.inputs()) {
          meshSurface.generateVertexBuffer(input);
      }
    }

    meshes.insert(id, modelMesh);
  }

  volume = Volume(lowerVertex, upperVertex);

  return meshes;
}

Sahara::ControllerDict Sahara::Model::parseColladaModelControllers(const QCollada::Collada& collada, MeshDict& meshes)
{
  Sahara::ControllerDict controllers;

  for (auto it = collada.controllers().begin(); it != collada.controllers().end(); it++) {
    QString id = it.key();
    QCollada::Controller* controller = it.value();
    Sahara::Mesh* mesh = meshes[controller->skin().source().mid(1)];

    const QCollada::Source& weightSource = controller->skin().getSource(
          controller->skin().vertexWeights().inputs()[QCollada::VertexWeights::Semantic::WEIGHT].first);
    const QCollada::FloatSource& weightSourceFloat = dynamic_cast<const QCollada::FloatSource&>(weightSource);

    const QCollada::Source& jointsSource = controller->skin().getSource( controller->skin().joints().inputs()[QCollada::Joints::Semantic::JOINT] );
    const QCollada::NameSource& jointsNameSource = dynamic_cast<const QCollada::NameSource&>(jointsSource);
    const QCollada::Source& ibmSource = controller->skin().getSource( controller->skin().joints().inputs()[QCollada::Joints::Semantic::INV_BIND_MATRIX] );
    const QCollada::FloatSource& ibmFloatSource = dynamic_cast<const QCollada::FloatSource&>(ibmSource);

    QStringList joints;
    QList<QMatrix4x4> inverseBindMatrices;
    for (int i = 0; i < jointsNameSource.data().size(); i++) {
      QString name = jointsNameSource.data().at(i);
      QList<float> ibmComponents = ibmFloatSource.data().mid(i * ibmFloatSource.accessor().stride(), ibmFloatSource.accessor().stride());
      QMatrix4x4 ibm;
      for (int j = 0; j < ibmComponents.size(); j += 4) {
        QVector4D row;
        row.setX(ibmComponents[j + 0]);
        row.setY(ibmComponents[j + 1]);
        row.setZ(ibmComponents[j + 2]);
        row.setW(ibmComponents[j + 3]);
        ibm.setRow(j / 4, row);
      }
      joints.append(name);
      inverseBindMatrices.append(ibm);
    }

    Sahara::Controller* modelController = new Sahara::Controller(
                id,
                mesh,
                controller->skin().bindShapeMatrix(),
                joints,
                inverseBindMatrices,
                weightSourceFloat.data(),
                controller->skin().vertexWeights().vcount(),
                controller->skin().vertexWeights().v());

    modelController->generateVertexBuffers();

    controllers.insert(id, modelController);
  }

  return controllers;
}

QList<Sahara::Instance*> Sahara::Model::parseColladaVisualScene(const QCollada::Collada& collada, const MaterialDict& materials, const MeshDict& meshes, const ControllerDict& controllers, Armature** const armaturePtr)
{
  QList<Instance*> instances;
  Sahara::Armature* armature = nullptr;

  const QCollada::VisualScene* visualScene;
  if ((visualScene = dynamic_cast<const QCollada::VisualScene*>( collada.resolve(collada.scene()->instanceVisualScene().url()) ))) {
    QStack<QMatrix4x4> transformStack;
    transformStack.push(QMatrix4x4());
    visualScene->root()->depthFirst([&](const QCollada::Node& node) {
      transformStack.push(transformStack.top() * node.transform());

      const QCollada::InstanceGeometry* instanceGeometry;
      const QCollada::InstanceController* instanceController;
      if ((instanceGeometry = dynamic_cast<const QCollada::InstanceGeometry*>(node.item()))) {
        Sahara::Mesh* mesh = meshes[instanceGeometry->url().mid(1)];

        QMap<QString, Material*> meshInstanceMaterials;
        for (const QCollada::InstanceMaterial& instanceMaterial : instanceGeometry->instanceMaterials()) {
          meshInstanceMaterials.insert(instanceMaterial.symbol(), materials[ instanceMaterial.target().mid(1) ]);
        }

        Sahara::InstanceMesh* meshInstance = new InstanceMesh(meshInstanceMaterials, transformStack.top(), mesh);
        instances.append(meshInstance);
      } else if ((instanceController = dynamic_cast<const QCollada::InstanceController*>(node.item()))) {
        Sahara::Controller* controller = controllers[instanceController->url().mid(1)];
        const QCollada::Node* armatureNode = visualScene->resolve(instanceController->skeleton());
        if (armature == nullptr)
          armature = parseColladaArmatureNode(*armatureNode);

        QMap<QString, Material*> controllerInstanceMaterials;
        for (const QCollada::InstanceMaterial& instanceMaterial : instanceController->instanceMaterials()) {
          controllerInstanceMaterials.insert(instanceMaterial.symbol(), materials[ instanceMaterial.target().mid(1) ]);
        }

        Sahara::InstanceController* controllerInstance = new InstanceController(armature, controllerInstanceMaterials, transformStack.top(), controller);
        instances.append(controllerInstance);
      }

      transformStack.pop();

      return false;
    });
  }

  *armaturePtr = armature;

  return instances;
}

Sahara::Armature* Sahara::Model::parseColladaArmatureNode(const QCollada::Node& rootNode)
{
  Sahara::Armature* armature = new Sahara::Armature("Armature", parseColladaJointNode(rootNode));

  return armature;
}

Sahara::Joint* Sahara::Model::parseColladaJointNode(const QCollada::Node& jointNode)
{
  Sahara::Joint* joint = new Sahara::Joint(nullptr, jointNode.id(), jointNode.sid(), Transform(jointNode.transform()));

  for (const QCollada::Node* node : jointNode.children()) {
    joint->addChild( parseColladaJointNode(*node) );
  }

  return joint;
}

Sahara::AnimationDict Sahara::Model::parseColladaModelAnimations(const QCollada::Collada& collada, Sahara::Armature& armature)
{
    Sahara::AnimationDict animations;

    for (auto it = collada.animations().begin(); it != collada.animations().end(); it++) {
      QString id = it.key();
      QCollada::Animation* animation = it.value();

      Sahara::Animation* modelAnimation = parseColladaModelAnimation(id, *animation, armature);

      animations.insert(id, modelAnimation);
    }

    return animations;
}

Sahara::AnimationClipDict Sahara::Model::parseColladaModelAnimationClips(const QCollada::Collada& collada, const AnimationDict& animations)
{
  Sahara::AnimationClipDict animationClips;

  if (collada.animationClips().isEmpty()) {
    if (!collada.animations().isEmpty()) {
      QList<Sahara::Animation*> clipAnimations;
      for (auto it = collada.animations().begin(); it != collada.animations().end(); it++) {
        QString id = it.key();
        Sahara::Animation* animation = animations[id];

        clipAnimations.append(animation);
      }

      Sahara::AnimationClip* modelAnimationClip = new Sahara::AnimationClip("Animation", "Animation", clipAnimations);
      animationClips.insert("Animation", modelAnimationClip);
    }
  } else {
    for (auto it = collada.animationClips().begin(); it != collada.animationClips().end(); it++) {
      QString id = it.key();
      QCollada::AnimationClip* animationClip = it.value();

      QList<Sahara::Animation*> clipAnimations;
      for (const QCollada::InstanceAnimation& instanceAnimation : animationClip->instanceAnimations()) {
        QString id = instanceAnimation.url().mid(1);

        Sahara::Animation* modelAnimation = animations[id];

        clipAnimations.append(modelAnimation);
      }

      Sahara::AnimationClip* modelAnimationClip = new Sahara::AnimationClip(id, animationClip->name(), clipAnimations);
      animationClips.insert(id, modelAnimationClip);
    }
  }

  return animationClips;
}

Sahara::Animation* Sahara::Model::parseColladaModelAnimation(const QString& id, const QCollada::Animation& animation, Sahara::Armature& armature)
{
  const QCollada::Source& inputSource = animation.getSource(animation.sampler().inputs()[QCollada::Sampler::Semantic::INPUT]);
  const QCollada::FloatSource& inputFloatSource = dynamic_cast<const QCollada::FloatSource&>(inputSource);
  const QCollada::Source& outputSource = animation.getSource(animation.sampler().inputs()[QCollada::Sampler::Semantic::OUTPUT]);
  const QCollada::FloatSource& outputFloatSource = dynamic_cast<const QCollada::FloatSource&>(outputSource);

  QList<Sahara::Animation::Keyframe> keyframes;
  for (int i = 0; i < inputFloatSource.accessor().count(); i++) {
    Sahara::Animation::Keyframe keyframe;

    keyframe.time = inputFloatSource.data()[i];

    QList<float> transformComponents = outputFloatSource.data().mid(i * outputSource.accessor().stride(), outputSource.accessor().stride());
    QMatrix4x4 transformMatrix;
    for (int j = 0; j < transformComponents.size(); j+= 4) {
      QVector4D row;
      row.setX(transformComponents[j + 0]);
      row.setY(transformComponents[j + 1]);
      row.setZ(transformComponents[j + 2]);
      row.setW(transformComponents[j + 3]);
      transformMatrix.setRow(j / 4, row);
    }

    keyframe.transform = Transform(transformMatrix);

    keyframes.append(keyframe);
  }
  QString jointId = animation.channel().target().split("/").at(0);
  Joint* joint = armature.getJointById(jointId);

  return new Sahara::Animation(id, joint, keyframes);
}
