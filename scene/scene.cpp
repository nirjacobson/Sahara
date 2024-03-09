#include "scene.h"
#include "render/renderer.h"

Sahara::Scene::Scene(Renderer *renderer)
  : _root(new Node("Root", nullptr, QMatrix4x4()))
  , _renderer(renderer)
  , _cameraNode(new Node(
                  "Camera",
                  new Camera("Camera", Camera::Mode::Perspective, 90, 7.31429f, 1.3333f, 1, 100),
                  QMatrix4x4(
                    0.9271839f, -0.1403301f, 0.3473292f, 5, 0, 0.9271839f, 0.3746066f, 5, -0.3746066f, -0.3473292f, 0.8596699f, 10, 0, 0, 0, 1
                  )))
{
    _root->addChild(_cameraNode);
    _lightingUniformBuffers = renderer->createLightingUniformBuffers();
}

Sahara::Scene::~Scene()
{
  for (Sahara::Camera* camera : _cameras.values()) {
    delete camera;
  }
  for (Sahara::Light* light : _lights.values()) {
    delete light;
  }
  for (Sahara::Model* model : _models.values()) {
    delete model;
  }
  delete _root;

  _renderer->destroyLightingUniformBuffers(_lightingUniformBuffers);
}

const Sahara::Node& Sahara::Scene::root() const
{
  return *_root;
}

Sahara::Node& Sahara::Scene::root()
{
  return *_root;
}


const QMap<QString, Sahara::Camera*>& Sahara::Scene::cameras() const
{
  return _cameras;
}

const QMap<QString, Sahara::Light*>& Sahara::Scene::lights() const
{
  return _lights;
}

const QMap<QString, Sahara::Model*>& Sahara::Scene::models() const
{
  return _models;
}

void Sahara::Scene::addCamera(const QString& id, Camera* const camera)
{
  _cameras.insert(id, camera);
}

void Sahara::Scene::addLight(const QString& id, Light* const light)
{
  _lights.insert(id, light);
}

void Sahara::Scene::addModel(const QString& id, Model* const model)
{
  _models.insert(id, model);
}

void Sahara::Scene::setCameraNode(Sahara::Node* node)
{
  _cameraNode = node;
}

Sahara::Node& Sahara::Scene::cameraNode()
{
  return *_cameraNode;
}

const Sahara::Node& Sahara::Scene::cameraNode() const
{
  return *_cameraNode;
}

Sahara::Camera& Sahara::Scene::camera()
{
  return dynamic_cast<Camera&>(_cameraNode->item());
}

const Sahara::Camera& Sahara::Scene::camera() const
{
  return dynamic_cast<Camera&>(_cameraNode->item());
}

Sahara::Node* Sahara::Scene::focusNode()
{
  Sahara::Node* focusNode = nullptr;
  _root->depthFirst([&](Sahara::Node& node) {
    if (node.hasFocus()) {
      focusNode = &node;
      return true;
    }
    return false;
  });

  return focusNode;
}

const Sahara::AmbientLight &Sahara::Scene::ambientLight() const
{
    return _ambientLight;
}

Sahara::AmbientLight& Sahara::Scene::ambientLight()
{
    return _ambientLight;
}

const QList<VkDescriptorSet>& Sahara::Scene::descriptorSets() const
{
    return _lightingUniformBuffers.bufferDescriptorSets;
}

void Sahara::Scene::updateUniform(const uint32_t currentFrame)
{
    ScenePipeline::Lighting lighting{
        .ambientLight = {
            .color = {
                _ambientLight.color().redF(),
                _ambientLight.color().greenF(),
                _ambientLight.color().blueF(),
            },
            .strength = _ambientLight.strength()
        },
        .pointLightCount = 0
    };

    _root->depthFirst([&](const Node& node) {
        const PointLight* pointLight;
        if ((pointLight = dynamic_cast<const PointLight*>(&node.item()))) {
            lighting.pointLights[lighting.pointLightCount++] = {
                .position = {
                    node.globalPosition().x(),
                    node.globalPosition().y(),
                    node.globalPosition().z()
                },
                .color = {
                    pointLight->color().redF(),
                    pointLight->color().greenF(),
                    pointLight->color().blueF()
                },
                .attenuation = {
                    pointLight->constantAttenuation(),
                    pointLight->linearAttenuation(),
                    pointLight->quadraticAttenuation(),
                }
            };
        }
        return false;
    });

    memcpy(_lightingUniformBuffers.buffersMapped[currentFrame], &lighting, sizeof(ScenePipeline::Lighting));

}




