#include "renderer.h"

Sahara::Renderer::Renderer(QVulkanWindow* vulkanWindow)
    : _vulkanWindow(vulkanWindow)
    , _deviceFunctions(vulkanWindow->vulkanInstance()->deviceFunctions(vulkanWindow->device()))
    , _grid(vulkanWindow, 32)
    , _showGrid(false)
    , _showAxes(true)
    , _showLights(true)
    , _showCameras(true)
    , _scenePipeline(vulkanWindow, VK_POLYGON_MODE_FILL)
    , _scenePipelineWire(vulkanWindow, VK_POLYGON_MODE_LINE)
    , _gridPipeline(vulkanWindow, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP)
    , _gridPipelineWire(vulkanWindow, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP)
    , _displayPipeline(vulkanWindow)
    , _scene(nullptr)
    , _paused(false)
    , _fps(0)
    , _pointLightDisplay(vulkanWindow)
    , _cameraDisplay(vulkanWindow)
{
    // can use any pipeline here because the render uniform block is the same
    _renderUniformBuffers = getUniformBuffers<GridPipeline::Render>(_gridPipeline, 0);
}

Sahara::Renderer::~Renderer()
{
    destroyUniformBuffers(_renderUniformBuffers);
}

void Sahara::Renderer::setScene(Scene* scene)
{
    _scene = scene;
}

// void Sahara::Renderer::render(const float time)
// {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     renderScene(_scene, time);
// }

bool Sahara::Renderer::showGrid() const
{
    return _showGrid;
}

void Sahara::Renderer::showGrid(const bool visible)
{
    _showGrid = visible;
}

bool Sahara::Renderer::showAxes() const
{
    return _showAxes;
}

void Sahara::Renderer::showAxes(const bool visible)
{
    _showAxes = visible;
}

bool Sahara::Renderer::showLights() const
{
    return _showLights;
}

void Sahara::Renderer::showLights(const bool visible)
{
    _showLights = visible;
}

bool Sahara::Renderer::showCameras() const
{
    return _showCameras;
}

void Sahara::Renderer::showCameras(const bool visible)
{
    _showCameras = visible;
}

void Sahara::Renderer::pause()
{
    _paused = true;
}

void Sahara::Renderer::resume()
{
    _paused = false;
    _vulkanWindow->requestUpdate();
}

float Sahara::Renderer::fps() const
{
    return _fps;
}

QVulkanWindow *Sahara::Renderer::window()
{
    return _vulkanWindow;
}

void Sahara::Renderer::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage &image, VkDeviceMemory &imageMemory)
{
    VulkanUtil::createImage(_vulkanWindow, width, height, format, tiling, usage, image, imageMemory);
}

VkImageView Sahara::Renderer::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    return VulkanUtil::createImageView(_vulkanWindow, image, format, aspectFlags);
}

QList<VkDescriptorSet> Sahara::Renderer::createImageDescriptorSets(VkImageView imageView)
{
    return _scenePipeline.createImageDescriptorSets(imageView);
}

void Sahara::Renderer::copyImage(const QImage& image, VkImage vkImage)
{
    const uint8_t* pixels = image.bits();
    VkDeviceSize imageSize = image.width() * image.height() * 4;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanUtil::createBuffer(_vulkanWindow, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, _vulkanWindow->hostVisibleMemoryIndex(), stagingBuffer, stagingBufferMemory);

    void* data;
    _deviceFunctions->vkMapMemory(_vulkanWindow->device(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    _deviceFunctions->vkUnmapMemory(_vulkanWindow->device(), stagingBufferMemory);

    VulkanUtil::transitionImageLayout(_vulkanWindow, vkImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    VulkanUtil::copyBufferToImage(_vulkanWindow, stagingBuffer, vkImage, image.width(), image.height());
    VulkanUtil::transitionImageLayout(_vulkanWindow, vkImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    _deviceFunctions->vkDestroyBuffer(_vulkanWindow->device(), stagingBuffer, nullptr);
    _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), stagingBufferMemory, nullptr);
}

VulkanUtil::UniformBuffers Sahara::Renderer::createArmatureUniformBuffers()
{
    return getUniformBuffers<ScenePipeline::Armature>(_scenePipeline, 1);
}

VulkanUtil::UniformBuffers Sahara::Renderer::createMaterialUniformBuffers()
{
    return getUniformBuffers<ScenePipeline::Material>(_scenePipeline, 3);
}

VulkanUtil::UniformBuffers Sahara::Renderer::createLightingUniformBuffers()
{
    return getUniformBuffers<ScenePipeline::Lighting>(_scenePipeline, 2);
}

void Sahara::Renderer::destroyUniformBuffers(VulkanUtil::UniformBuffers &buffers)
{
    for (int i = 0; i < buffers.buffers.size(); i++) {
        _deviceFunctions->vkUnmapMemory(_vulkanWindow->device(), buffers.buffersMemory[i]);
        _deviceFunctions->vkDestroyBuffer(_vulkanWindow->device(), buffers.buffers[i], nullptr);
        _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), buffers.buffersMemory[i], nullptr);
    }
}

template <typename T>
VulkanUtil::UniformBuffers Sahara::Renderer::getUniformBuffers(Pipeline &pipeline, uint32_t binding)
{
    int concurrentFrames = _vulkanWindow->concurrentFrameCount();

    VulkanUtil::UniformBuffers buffers;
    buffers.resize(concurrentFrames);

    for (int i = 0; i < concurrentFrames; i++) {
        VulkanUtil::createBuffer(_vulkanWindow, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, _vulkanWindow->hostVisibleMemoryIndex(), buffers.buffers[i], buffers.buffersMemory[i]);
        _deviceFunctions->vkMapMemory(_vulkanWindow->device(), buffers.buffersMemory[i], 0, sizeof(T), 0, &buffers.buffersMapped[i]);
    }
    buffers.bufferDescriptorSets = pipeline.createBufferDescriptorSets(binding, buffers.buffers, sizeof(GridPipeline::Render));

    return buffers;
}

void Sahara::Renderer::recordGrid(Scene &scene)
{
    _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipelineWire.pipeline());
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipelineWire.pipelineLayout(), 0, 1, &_renderUniformBuffers.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

    QList<VkBuffer> vertexBuffers = _grid.buffersByBinding(_gridPipelineWire);
    QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
    _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());

    for (int i = 0; i < 2 * _grid.length(); i++) {
        _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 5, 1, 5 * i, 0);
    }

    if (_showAxes) {
        _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipeline.pipeline());
        _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipeline.pipelineLayout(), 0, 1, &_renderUniformBuffers.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

        QList<VkBuffer> vertexBuffers = _grid.xAxis().buffersByBinding(_gridPipeline);
        QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
        _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
        for (int i = 0; i < 6; i++) {
            _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 4, 1, 4 * i, 0);
        }

        vertexBuffers = _grid.yAxis().buffersByBinding(_gridPipeline);
        offsets = QList<VkDeviceSize>(vertexBuffers.size(), 0);
        _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
        for (int i = 0; i < 6; i++) {
            _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 4, 1, 4 * i, 0);
        }

        vertexBuffers = _grid.zAxis().buffersByBinding(_gridPipeline);
        offsets = QList<VkDeviceSize>(vertexBuffers.size(), 0);
        _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
        for (int i = 0; i < 6; i++) {
            _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 4, 1, 4 * i, 0);
        }
    }
}

void Sahara::Renderer::recordDisplay(Scene &scene, Display &display, const QMatrix4x4 &modelView, const bool focus)
{
    _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _displayPipeline.pipeline());
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _displayPipeline.pipelineLayout(), 0, 1, &_renderUniformBuffers.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

    float modelViewF[16];
    modelView.copyDataTo(modelViewF);
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _displayPipeline.pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(DisplayPipeline::PushConstants, modelView), sizeof(modelViewF), modelViewF);

    QVector3D cameraPosVec = scene.cameraNode().globalPosition();
    float cameraPositionF[] = {
        cameraPosVec.x(),
        cameraPosVec.y(),
        cameraPosVec.z()
    };
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _displayPipeline.pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(DisplayPipeline::PushConstants, cameraPosition), sizeof(cameraPositionF), cameraPositionF);

    int f = focus;
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _displayPipeline.pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(DisplayPipeline::PushConstants, focus), sizeof(f), &f);

    QList<VkBuffer> vertexBuffers = display.buffersByBinding(_displayPipeline);
    QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
    _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());

    for (int i = 0; i < display.count() / 3; i++) {
        _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 4, 1, 0, 0);
    }
}

void Sahara::Renderer::recordPointLight(Scene &scene, const QMatrix4x4& modelView, const bool focus)
{
    recordDisplay(scene, _pointLightDisplay, modelView, focus);
}

void Sahara::Renderer::recordCamera(Scene &scene, const QMatrix4x4 &modelView, const bool focus)
{
    recordDisplay(scene, _cameraDisplay, modelView, focus);
}

void Sahara::Renderer::recordScene(Scene &scene, const float time)
{
    if (_showGrid) {
        recordGrid(scene);
    }

    scene.updateUniform();

    VkDescriptorSet descriptorSet = scene.descriptorSets()[_vulkanWindow->currentFrame()];
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _scenePipeline.pipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

    QStack<QMatrix4x4> transforms;
    transforms.push(QMatrix4x4());
    scene.root().depthFirst([&](Node& node) {
       transforms.push(transforms.top() * node.transform());
       Model* model;
       PointLight* pointLight;
       Camera* camera;
       if ((model = dynamic_cast<Model*>(&node.item()))) {
           recordModel(*model, transforms, false, time);
           if (node.hasFocus()) {
               recordModel(*model, transforms, true, time);
           }
       } else {
           if ((pointLight = dynamic_cast<PointLight*>(&node.item()))) {
               if (_showLights) {
                   recordPointLight(scene, transforms.top(), node.hasFocus());
               }
           } else if ((camera = dynamic_cast<Camera*>(&node.item()))) {
               if (_showCameras && &node != &scene.cameraNode()) {
                   recordCamera(scene, transforms.top(), node.hasFocus());
               }
           }
       }

       return false;
    }, [&](Node&) {
        transforms.pop();
        return false;
     });
}

void Sahara::Renderer::recordSurface(Surface &surface, Instance &instance, const bool focus)
{
    const Material& material = instance.getMaterial(surface.material());

    material.updateUniform();

    VkDescriptorSet descriptorSet = material.descriptorSets()[_vulkanWindow->currentFrame()];
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _scenePipeline.pipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

    if (material.image().has_value()) {
        VkDescriptorSet descriptorSet = (*material.image())->descriptorSets()[_vulkanWindow->currentFrame()];
        _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _scenePipeline.pipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);
    }

    QList<VkBuffer> vertexBuffers = surface.buffersByBinding(_scenePipeline);
    QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
    _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());

    _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), surface.count(), 1, 0, 0);
}

void Sahara::Renderer::recordModel(Model &model, QStack<QMatrix4x4> &transformStack, const bool focus, const float time)
{
    _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, (focus ? _scenePipelineWire : _scenePipeline).pipeline());
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _scenePipeline.pipelineLayout(), 0, 1, &_renderUniformBuffers.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

    int f = focus;
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _scenePipeline.pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(ScenePipeline::PushConstants, focus), sizeof(f), &f);

    model.animate(time);

    for (Instance* instance : model.instances()) {
        transformStack.push(transformStack.top() * instance->transform());

        float modelViewF[16];
        transformStack.top().copyDataTo(modelViewF);
        _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _scenePipeline.pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(ScenePipeline::PushConstants, modelView), sizeof(modelViewF), modelViewF);
        InstanceMesh* meshInstance;
        InstanceController* controllerInstance;
        if ((meshInstance = dynamic_cast<InstanceMesh*>(instance))) {
            int articulated = false;
            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _scenePipeline.pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(ScenePipeline::PushConstants, articulated), sizeof(articulated), &articulated);

            for (int i = 0; i < meshInstance->mesh().count(); i++) {
                recordSurface(meshInstance->mesh().surface(i), *meshInstance, focus);
            }
        } else if ((controllerInstance = dynamic_cast<InstanceController*>(instance))) {
            int articulated = true;
            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _scenePipeline.pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(ScenePipeline::PushConstants, articulated), sizeof(articulated), &articulated);

            controllerInstance->updateUniform();

            VkDescriptorSet descriptorSet = controllerInstance->descriptorSets()[_vulkanWindow->currentFrame()];
            _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _scenePipeline.pipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

            for (int i = 0; i < controllerInstance->controller().mesh().count(); i++) {
                recordSurface(controllerInstance->controller().mesh().surface(i), *controllerInstance, focus);
            }
        }

        transformStack.pop();
    }
}

void Sahara::Renderer::record(const float time)
{
    // Update Render uniform used in all pipelines
    GridPipeline::Render render;
    _scene->cameraNode().globalTransform().inverted().copyDataTo(render.inverseCamera);
    (_vulkanWindow->clipCorrectionMatrix() * _scene->camera().projection()).copyDataTo(render.projection);
    memcpy(_renderUniformBuffers.buffersMapped[_vulkanWindow->currentFrame()], &render, sizeof(GridPipeline::Render));

    recordScene(*_scene, time);
}

// void Sahara::Renderer::renderScene(Scene& scene, const float time)
// {
//     if (_showGrid)
//         renderGrid(scene);

//     _sceneProgram.bind();

//     _sceneProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
//     _sceneProgram.setCameraPosition(scene.cameraNode().globalPosition());
//     _sceneProgram.setProjection(scene.camera().projection());

//     processSceneLighting(scene);

//     QStack<QMatrix4x4> transforms;
//     transforms.push(QMatrix4x4());
//     scene.root().depthFirst([&](Node& node) {
//        transforms.push(transforms.top() * node.transform());
//        Model* model;
//        PointLight* pointLight;
//        Camera* camera;
//        if ((model = dynamic_cast<Model*>(&node.item()))) {
//            _sceneProgram.setFocus(false);
//            renderModel(*model, transforms, false, time);
//            if (node.hasFocus()) {
//                _sceneProgram.setFocus(true);
//                renderModel(*model, transforms, true, time);
//            }
//        } else {
//            _sceneProgram.release();

//            if ((pointLight = dynamic_cast<PointLight*>(&node.item()))) {
//                if (_showLights) {
//                    renderPointLight(scene, transforms.top(), node.hasFocus());
//                }
//            } else if ((camera = dynamic_cast<Camera*>(&node.item()))) {
//                if (_showCameras && &node != &scene.cameraNode()) {
//                    renderCamera(scene, transforms.top(), node.hasFocus());
//                }
//            }

//            _sceneProgram.bind();
//        }

//        return false;
//     }, [&](Node&) {
//         transforms.pop();
//         return false;
//      });

//     _sceneProgram.release();
// }

// void Sahara::Renderer::renderGrid(Scene& scene)
// {
//     _gridProgram.bind();

//     _gridProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
//     _gridProgram.setProjection(scene.camera().projection());

//     _gridProgram.setGrid(_grid);
//     for (int i = 0; i < 2 * _grid.length(); i++) {
//         glDrawArrays(GL_LINE_LOOP, i * 4, 4);
//     }
//     _gridProgram.clearGrid(_grid);

//     if (_showAxes) {
//         _gridProgram.setAxis(_grid.xAxis());
//         for (int i = 0; i < 6; i++) {
//             glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
//         }
//         _gridProgram.clearAxis(_grid.xAxis());

//         _gridProgram.setAxis(_grid.yAxis());
//         for (int i = 0; i < 6; i++) {
//             glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
//         }
//         _gridProgram.clearAxis(_grid.yAxis());

//         _gridProgram.setAxis(_grid.zAxis());
//         for (int i = 0; i < 6; i++) {
//             glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
//         }
//         _gridProgram.clearAxis(_grid.zAxis());
//     }

//     _gridProgram.release();
// }

// void Sahara::Renderer::renderPointLight(Scene& scene, const QMatrix4x4& modelView, const bool focus)
// {
//     _displayProgram.bind();

//     _displayProgram.setModelView(modelView);
//     _displayProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
//     _displayProgram.setProjection(scene.camera().projection());
//     _displayProgram.setCameraPosition(scene.cameraNode().globalPosition());
//     _displayProgram.setFocus(focus);

//     _displayProgram.setDisplay(_pointLightDisplay);

//     VertexBuffer buffer = _pointLightDisplay.vertexBuffers().first();
//     int vertices = buffer.count();

//     for (int i = 0; i < vertices / 3; i++) {
//         glDrawArrays(GL_LINE_LOOP, i * 3, 3);
//     }

//     _displayProgram.clearDisplay(_pointLightDisplay);

//     _displayProgram.release();
// }

// void Sahara::Renderer::renderCamera(Sahara::Scene& scene, const QMatrix4x4& modelView, const bool focus)
// {
//     _displayProgram.bind();

//     _displayProgram.setModelView(modelView);
//     _displayProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
//     _displayProgram.setProjection(scene.camera().projection());
//     _displayProgram.setCameraPosition(scene.cameraNode().globalPosition());
//     _displayProgram.setFocus(focus);

//     _displayProgram.setDisplay(_cameraDisplay);

//     VertexBuffer buffer = _cameraDisplay.vertexBuffers().first();
//     int vertices = buffer.count();

//     for (int i = 0; i < vertices / 3; i++) {
//         glDrawArrays(GL_LINE_LOOP, i * 3, 3);
//     }

//     _displayProgram.clearDisplay(_cameraDisplay);

//     _displayProgram.release();
// }

// void Sahara::Renderer::renderModel(Sahara::Model& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time)
// {
//     model.animate(time);

//     for (Instance* instance : model.instances()) {
//         transformStack.push(transformStack.top() * instance->transform());
//         _sceneProgram.setModelView(transformStack.top());

//         InstanceMesh* meshInstance;
//         InstanceController* controllerInstance;
//         if ((meshInstance = dynamic_cast<InstanceMesh*>(instance))) {
//             _sceneProgram.setArticulated(false);

//             for (int i = 0; i < meshInstance->mesh().count(); i++) {
//                 renderSurface(meshInstance->mesh().surface(i), *meshInstance, focus);
//             }
//         } else if ((controllerInstance = dynamic_cast<InstanceController*>(instance))) {
//             _sceneProgram.setArticulated(true);

//             processControllerInstanceArmature(*controllerInstance);

//             for (int i = 0; i < controllerInstance->controller().mesh().count(); i++) {
//                 renderSurface(controllerInstance->controller().mesh().surface(i), *controllerInstance, focus);
//             }
//         }

//         transformStack.pop();
//     }
// }

// void Sahara::Renderer::renderSurface(Sahara::Surface& surface, Instance& instance, const bool focus)
// {
//     const Material& material = instance.getMaterial(surface.material());
//     _sceneProgram.setMaterial(material);
//     if (material.image().has_value()) {
//         (*material.image())->bind();
//     }

//     _sceneProgram.setSurface(surface);
//     if (focus) {
//         VertexBuffer buffer = surface.vertexBuffers().first();
//         int vertices = buffer.count();

//         for (int i = 0; i < vertices / 3; i++) {
//             glDrawArrays(GL_LINE_LOOP, i * 3, 3);
//         }
//     } else {
//         glDrawArrays(GL_TRIANGLES, 0, surface.vertexBuffers().first().count());
//     }
//    _sceneProgram.clearSurface(surface);

//    assert(glGetError() == GL_NO_ERROR);
// }

// void Sahara::Renderer::processSceneLighting(Sahara::Scene& scene)
// {
//     _sceneProgram.setAmbientLight(scene.ambientLight());
//     _sceneProgram.clearPointLights();
//     scene.root().depthFirst([&](const Node& node) {
//         const PointLight* pointLight;
//         if ((pointLight = dynamic_cast<const PointLight*>(&node.item()))) {
//             _sceneProgram.addPointLight(*pointLight, node.globalPosition());
//         }
//         return false;
//     });
// }

// void Sahara::Renderer::processControllerInstanceArmature(Sahara::InstanceController& controllerInstance)
// {
//     const Controller& controller = controllerInstance.controller();

//     QList<Transform> jointTransforms;
//     for (int i = 0; i < controller.joints().size(); i++) {
//         QString jointName = controller.joints().at(i);
//         const Joint* joint = controllerInstance.armature().getJointByName(jointName);
//         QMatrix4x4 inverseBindMatrix = controller.inverseBindMatrices().at(i);
//         QMatrix4x4 matrix = inverseBindMatrix * controller.bindShapeMatrix();

//         Transform transform = Transform(matrix);

//         while (joint) {
//             transform = joint->transform() * transform;
//             joint = &joint->parent();
//         }

//         jointTransforms.push_back(transform);
//     }

//     _sceneProgram.setJointTransforms(jointTransforms);
// }

void Sahara::Renderer::startNextFrame()
{
    if (_scene) {
        VkDevice dev = _vulkanWindow->device();
        VkCommandBuffer cb = _vulkanWindow->currentCommandBuffer();
        const QSize sz = _vulkanWindow->swapChainImageSize();

        VkClearColorValue clearColor = {{ 0.5f, 0.75f, 0.86f, 1.0f }};
        VkClearDepthStencilValue clearDS = { 1, 0 };
        VkClearValue clearValues[3];
        memset(clearValues, 0, sizeof(clearValues));
        clearValues[0].color = clearValues[2].color = clearColor;
        clearValues[1].depthStencil = clearDS;

        VkRenderPassBeginInfo rpBeginInfo;
        memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
        rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBeginInfo.renderPass = _vulkanWindow->defaultRenderPass();
        rpBeginInfo.framebuffer = _vulkanWindow->currentFramebuffer();
        rpBeginInfo.renderArea.extent.width = sz.width();
        rpBeginInfo.renderArea.extent.height = sz.height();
        rpBeginInfo.clearValueCount = _vulkanWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
        rpBeginInfo.pClearValues = clearValues;
        VkCommandBuffer cmdBuf = _vulkanWindow->currentCommandBuffer();
        _deviceFunctions->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(sz.width()),
            .height = static_cast<float>(sz.height()),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
        _deviceFunctions->vkCmdSetViewport(cmdBuf, 0, 1, &viewport);

        VkRect2D scissor{
            .offset = {
                .x = 0,
                .y = 0
            },
            .extent = {
                .width = static_cast<uint32_t>(sz.width()),
                .height = static_cast<uint32_t>(sz.height()),
            }
        };
        _deviceFunctions->vkCmdSetScissor(cmdBuf, 0, 1, &scissor);

        record(0);

        _vulkanWindow->frameReady();

        if (!_paused) {
            _vulkanWindow->requestUpdate();
        }
    }

    _fps = 1000.0 / _frameTime.restart();
}

void Sahara::Renderer::initSwapChainResources()
{
    // Update Render uniform used in all pipelines
    GridPipeline::Render render;
    _scene->cameraNode().globalTransform().inverted().copyDataTo(render.inverseCamera);
    (_vulkanWindow->clipCorrectionMatrix() * _scene->camera().projection()).copyDataTo(render.projection);
    memcpy(_renderUniformBuffers.buffersMapped[_vulkanWindow->currentFrame()], &render, sizeof(GridPipeline::Render));
}
