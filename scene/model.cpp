#include "model.h"


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
  Sahara::AnimationClipDict animationClips = parseColladaModelAnimationClips(collada, *armature);

  model->_images = images;
  model->_materials = materials;
  model->_meshes = meshes;
  model->_controllers = controllers;
  model->_armature = armature;
  model->_instances = instances;
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
    Sahara::Image* modelImage = new Sahara::Image(QDir::cleanPath(path + QDir::separator() + image->initFrom()));

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
          effect->phong().emission(),
          effect->phong().ambient(),
          image,
          effect->phong().specular(),
          effect->phong().shininess());
      } else {
        modelMaterial = new Sahara::Material(
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

  QVector3D leastVertex;
  QVector3D greatestVertex;

  for (auto it = collada.geometries().begin(); it != collada.geometries().end(); it++) {
    QString id = it.key();
    QCollada::Geometry* geometry = it.value();

    QList<Sahara::Surface> surfaces;
    for (const QCollada::Triangles& triangles : geometry->mesh().triangles()) {
      Surface surface(triangles.material());
      for (auto it = triangles.inputs().begin(); it != triangles.inputs().end(); ++it) {
        QCollada::Triangles::Semantic semantic = it.key();
        QString url = it.value().first;
        int offset = it.value().second;

        const QCollada::Source& source = (semantic == QCollada::Triangles::Semantic::VERTEX)
          ? geometry->mesh().getSource(geometry->mesh().vertices().inputs().begin().value())
          : geometry->mesh().getSource(url);
        const QCollada::FloatSource& floatSource = dynamic_cast<const QCollada::FloatSource&>(source);

        int elemsPerBuffer = triangles.count() * 3;
        int dataSize = elemsPerBuffer * floatSource.accessor().stride() * static_cast<int>(sizeof(GLfloat));
        int dataIndex = 0;
        char* data = new char[static_cast<unsigned long>(dataSize)];

        for (int i = 0; i < elemsPerBuffer * triangles.inputs().size(); i += triangles.inputs().size()) {
          int sourceIndex = triangles.p()[i+offset];
          int sourceElementIndex = sourceIndex * source.accessor().stride();

          for (int j = 0; j < floatSource.accessor().stride(); j++) {
            *(reinterpret_cast<GLfloat*>(data) + dataIndex++) = floatSource.data()[sourceElementIndex + j];
          }

          if (semantic == QCollada::Triangles::Semantic::VERTEX) {
            for (int j = 0; j < 3; j++) {
              float component = floatSource.data()[sourceElementIndex + j];
              if (component < leastVertex[j]) {
                switch (j) {
                  case 0: leastVertex.setX(component); break;
                  case 1: leastVertex.setY(component); break;
                  case 2: leastVertex.setZ(component); break;
                }
              }
              if (component > greatestVertex[j]) {
                switch (j) {
                  case 0: greatestVertex.setX(component); break;
                  case 1: greatestVertex.setY(component); break;
                  case 2: greatestVertex.setZ(component); break;
                }
              }
            }
          }
        }

        QString bufferName = ((semantic == QCollada::Triangles::Semantic::VERTEX)
            ? QCollada::Vertices::semanticToString( geometry->mesh().vertices().inputs().begin().key() )
            : QCollada::Triangles::semanticToString(semantic)).toLower();

        surface.addVertexBuffer(bufferName, GL_FLOAT, data, dataSize, floatSource.accessor().stride());
        delete [] data;
      }
      surfaces.append(surface);
    }

    Sahara::Mesh* modelMesh = new Sahara::Mesh(surfaces);

    meshes.insert(id, modelMesh);
  }

  volume = Volume(leastVertex, greatestVertex);

  return meshes;
}

QList<QPair<int, float>> Sahara::Model::reduceBones(const QList<QPair<int, float> >& bones, const int max)
{
    auto sortBones =  [](const QPair<int, float>& b1, const QPair<int, float>& b2) {
        return b1.second > b2.second;
    };

    QList<QPair<int, float>> sortedBones = bones;
    std::sort(sortedBones.begin(), sortedBones.end(), sortBones);


    QList<QPair<int, float>> reducedBones = sortedBones.mid(0, max);

    float totalWeight = 0;
    for (const QPair<int, float>& bone : reducedBones) {
        totalWeight += bone.second;
    }
    for (QPair<int, float>& bone : reducedBones) {
        bone.second /= totalWeight;
    }

    return reducedBones;
}

Sahara::ControllerDict Sahara::Model::parseColladaModelControllers(const QCollada::Collada& collada, MeshDict& meshes)
{
  Sahara::ControllerDict controllers;

  for (auto it = collada.controllers().begin(); it != collada.controllers().end(); it++) {
    QString id = it.key();
    QCollada::Controller* controller = it.value();
    const QCollada::Asset* geometryAsset = collada.resolve(controller->skin().source());
    const QCollada::Geometry* geometry = dynamic_cast<const QCollada::Geometry*>(geometryAsset);
    Sahara::Mesh* mesh = meshes[controller->skin().source().mid(1)];

    const QCollada::Source& weightSource = controller->skin().getSource(
          controller->skin().vertexWeights().inputs()[QCollada::VertexWeights::Semantic::WEIGHT].first);
    const QCollada::FloatSource& weightSourceFloat = dynamic_cast<const QCollada::FloatSource&>(weightSource);

    QList<QList<QPair<int, float>>> verticesBonesAndWeights;
    int vIndex = 0;
    for (int i = 0; i < controller->skin().vertexWeights().vcount().size(); i++) {
      int numBones = controller->skin().vertexWeights().vcount()[i];

      QList<QPair<int, float>> vertexBonesAndWeights;
      for (int j = 0; j < numBones; j++) {
        vertexBonesAndWeights.append( QPair<int, float>(
          controller->skin().vertexWeights().v()[vIndex+0],
          weightSourceFloat.data()[ controller->skin().vertexWeights().v()[vIndex+1] ]
        ) );
        vIndex += 2;
      }

      verticesBonesAndWeights.append( vertexBonesAndWeights );
    }

    for (int i = 0; i < mesh->surfaces().size(); i++) {
      Sahara::Surface& surface = mesh->surfaces()[i];
      const QCollada::Triangles& triangles = geometry->mesh().triangles().at(i);

      int elemsPerBuffer = triangles.count() * 3;
      int bonesDataSize = elemsPerBuffer * 4 * static_cast<int>(sizeof(GLfloat));
      char* bones1Data = new char[static_cast<unsigned long>(bonesDataSize)];
      char* bones2Data = new char[static_cast<unsigned long>(bonesDataSize)];
      int weightsDataSize = elemsPerBuffer * 4 * static_cast<int>(sizeof(GLfloat));
      char* weights1Data = new char[static_cast<unsigned long>(weightsDataSize)];
      char* weights2Data = new char[static_cast<unsigned long>(weightsDataSize)];
      int bones1DataIndex = 0;
      int bones2DataIndex = 0;
      int weights1DataIndex = 0;
      int weights2DataIndex = 0;

      int pOffset = triangles.inputs()[QCollada::Triangles::Semantic::VERTEX].second;
      int pStride = triangles.inputs().size();
      for (int j = 0; j < triangles.p().size(); j += pStride) {
        int vertex = triangles.p()[j + pOffset];
        const QList<QPair<int, float>>& vertexBonesAndWeights = reduceBones(verticesBonesAndWeights.at(vertex), 8);

        for (int k = 0; k < 8; k++) {
          if (k < vertexBonesAndWeights.size()) {
            const QPair<int, float>& boneAndWeight = vertexBonesAndWeights.at(k);

            if (k < 4) {
                reinterpret_cast<GLfloat*>(bones1Data)[bones1DataIndex] = boneAndWeight.first;
                reinterpret_cast<GLfloat*>(weights1Data)[weights1DataIndex] = boneAndWeight.second;
            } else {
                reinterpret_cast<GLfloat*>(bones2Data)[bones2DataIndex] = boneAndWeight.first;
                reinterpret_cast<GLfloat*>(weights2Data)[weights2DataIndex] = boneAndWeight.second;
            }
          } else {
              if (k < 4) {
                  reinterpret_cast<GLfloat*>(bones1Data)[bones1DataIndex] = -1;
                  reinterpret_cast<GLfloat*>(weights1Data)[weights1DataIndex] = -1;
              } else {
                  reinterpret_cast<GLfloat*>(bones2Data)[bones2DataIndex] = -1;
                  reinterpret_cast<GLfloat*>(weights2Data)[weights2DataIndex] = -1;
              }
          }

          if (k < 4) {
              bones1DataIndex++;
              weights1DataIndex++;
          } else {
              bones2DataIndex++;
              weights2DataIndex++;
          }
        }
      }

      surface.addVertexBuffer("bones1", GL_FLOAT, bones1Data, bonesDataSize, 4);
      surface.addVertexBuffer("bones2", GL_FLOAT, bones2Data, bonesDataSize, 4);
      surface.addVertexBuffer("weights1", GL_FLOAT, weights1Data, weightsDataSize, 4);
      surface.addVertexBuffer("weights2", GL_FLOAT, weights2Data, weightsDataSize, 4);

      delete [] bones1Data;
      delete [] bones2Data;
      delete [] weights1Data;
      delete [] weights2Data;
    }


    const QCollada::Source& jointsSource = controller->skin().getSource( controller->skin().joints().inputs()[QCollada::Joints::Semantic::JOINT] );
    const QCollada::NameSource& jointsNameSource = dynamic_cast<const QCollada::NameSource&>(jointsSource);
    const QCollada::Source& ibmSource = controller->skin().getSource( controller->skin().joints().inputs()[QCollada::Joints::Semantic::INV_BIND_MATRIX] );
    const QCollada::FloatSource& ibmFloatSource = dynamic_cast<const QCollada::FloatSource&>(ibmSource);

    QStringList bones;
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
      bones.append(name);
      inverseBindMatrices.append(ibm);
    }

    Sahara::Controller* modelController = new Sahara::Controller(mesh, controller->skin().bindShapeMatrix(), bones, inverseBindMatrices);

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
    visualScene->root()->depthFirst([&](const QCollada::Node& node, auto) {
      transformStack.push(transformStack.top() * node.transform());

      const QCollada::InstanceGeometry* instanceGeometry;
      const QCollada::InstanceController* instanceController;
      if ((instanceGeometry = dynamic_cast<const QCollada::InstanceGeometry*>(node.item()))) {
        Sahara::Mesh* mesh = meshes[instanceGeometry->url().mid(1)];

        QMap<QString, Material*> meshInstanceMaterials;
        for (const QCollada::InstanceMaterial& instanceMaterial : instanceGeometry->instanceMaterials()) {
          meshInstanceMaterials.insert(instanceMaterial.symbol(), materials[ instanceMaterial.target().mid(1) ]);
        }

        Sahara::MeshInstance* meshInstance = new MeshInstance(meshInstanceMaterials, transformStack.top(), mesh);
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

        Sahara::ControllerInstance* controllerInstance = new ControllerInstance(armature, controllerInstanceMaterials, transformStack.top(), controller);
        instances.append(controllerInstance);
      }

      transformStack.pop();
    });
  }

  *armaturePtr = armature;

  return instances;
}

Sahara::Armature* Sahara::Model::parseColladaArmatureNode(const QCollada::Node& rootNode)
{
  Sahara::Armature* armature = new Sahara::Armature( parseColladaBoneNode(rootNode) );

  return armature;
}

Sahara::Bone* Sahara::Model::parseColladaBoneNode(const QCollada::Node& boneNode)
{
  Sahara::Bone* bone = new Sahara::Bone(nullptr, boneNode.id(), boneNode.sid(), boneNode.transform());

  for (const QCollada::Node* node : boneNode.children()) {
    bone->addChild( parseColladaBoneNode(*node) );
  }

  return bone;
}

Sahara::AnimationClipDict Sahara::Model::parseColladaModelAnimationClips(const QCollada::Collada& collada, Sahara::Armature& armature)
{
  Sahara::AnimationClipDict animationClips;

  if (collada.animationClips().isEmpty()) {
    if (!collada.animations().isEmpty()) {
      QList<Sahara::Animation*> clipAnimations;
      for (auto it = collada.animations().begin(); it != collada.animations().end(); it++) {
        QCollada::Animation* animation = it.value();

        Sahara::Animation* modelAnimation = parseColladaModelAnimation(*animation, armature);

        clipAnimations.append(modelAnimation);
      }

      Sahara::AnimationClip* modelAnimationClip = new Sahara::AnimationClip("Animation", clipAnimations);
      animationClips.insert("Animation", modelAnimationClip);
    }
  } else {
    for (auto it = collada.animationClips().begin(); it != collada.animationClips().end(); it++) {
      QString id = it.key();
      QCollada::AnimationClip* animationClip = it.value();

      QList<Sahara::Animation*> clipAnimations;
      for (const QCollada::InstanceAnimation& instanceAnimation : animationClip->instanceAnimations()) {
        QCollada::Animation* animation = collada.animations()[instanceAnimation.url().mid(1)];

        Sahara::Animation* modelAnimation = parseColladaModelAnimation(*animation, armature);

        clipAnimations.append(modelAnimation);
      }

      Sahara::AnimationClip* modelAnimationClip = new Sahara::AnimationClip(animationClip->name(), clipAnimations);
      animationClips.insert(id, modelAnimationClip);
    }
  }

  return animationClips;
}

Sahara::Animation* Sahara::Model::parseColladaModelAnimation(const QCollada::Animation& animation, Sahara::Armature& armature)
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
    QMatrix4x4 transform;
    for (int j = 0; j < transformComponents.size(); j+= 4) {
      QVector4D row;
      row.setX(transformComponents[j + 0]);
      row.setY(transformComponents[j + 1]);
      row.setZ(transformComponents[j + 2]);
      row.setW(transformComponents[j + 3]);
      transform.setRow(j / 4, row);
    }

    float rotationValues[] = {
      transform.row(0).x(), transform.row(0).y(), transform.row(0).z(),
      transform.row(1).x(), transform.row(1).y(), transform.row(1).z(),
      transform.row(2).x(), transform.row(2).y(), transform.row(2).z(),
    };

    keyframe.rotation = QQuaternion::fromRotationMatrix(QMatrix3x3(rotationValues));
    keyframe.translation = QVector3D(transform.column(3));

    keyframes.append(keyframe);
  }
  QString boneId = animation.channel().target().split("/").at(0);
  Bone* bone = armature.getBoneById(boneId);

  return new Sahara::Animation(bone, keyframes);
}
