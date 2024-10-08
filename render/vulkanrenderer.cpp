#include "vulkanrenderer.h"

Sahara::VulkanRenderer::VulkanRenderer(QVulkanWindow *vulkanWindow)
    : _vulkanWindow(vulkanWindow)
    , _scene(nullptr)
    , _paused(false)
    , _showFPS(true)
{

}

void Sahara::VulkanRenderer::setScene(VulkanScene* scene)
{
    _scene = scene;
}

bool Sahara::VulkanRenderer::showFPS() const
{
    return _showFPS;
}

void Sahara::VulkanRenderer::showFPS(const bool visible)
{
    _showFPS = visible;
}

void Sahara::VulkanRenderer::pause()
{
    _paused = true;
}

void Sahara::VulkanRenderer::resume()
{
    _paused = false;
    _vulkanWindow->requestUpdate();
}

float Sahara::VulkanRenderer::fps() const
{
    return _fps;
}

QVulkanWindow *Sahara::VulkanRenderer::window()
{
    return _vulkanWindow;
}

void Sahara::VulkanRenderer::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage &image, VkDeviceMemory &imageMemory)
{
    VulkanUtil::createImage(_vulkanWindow, width, height, format, tiling, usage, image, imageMemory);
}

VkImageView Sahara::VulkanRenderer::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    return VulkanUtil::createImageView(_vulkanWindow, image, format, aspectFlags);
}

QList<VkDescriptorSet> Sahara::VulkanRenderer::createImageDescriptorSets(VkImageView imageView)
{
    return _scenePipeline->createImageDescriptorSets(imageView);
}

void Sahara::VulkanRenderer::freeImageDescriptorSets(const QList<VkDescriptorSet> &descriptorSets)
{
    _deviceFunctions->vkQueueWaitIdle(_vulkanWindow->graphicsQueue());
    _scenePipeline->freeDescriptorSets(descriptorSets);
}

void Sahara::VulkanRenderer::freePanelImageDescriptorSets(const QList<VkDescriptorSet> &descriptorSets)
{
    _deviceFunctions->vkQueueWaitIdle(_vulkanWindow->graphicsQueue());
    _panelPipeline->freeDescriptorSets(descriptorSets);
}

void Sahara::VulkanRenderer::destroyImage(VkImage image, VkDeviceMemory memory, VkImageView imageView)
{
    _deviceFunctions->vkDestroyImageView(_vulkanWindow->device(), imageView, nullptr);
    _deviceFunctions->vkDestroyImage(_vulkanWindow->device(), image, nullptr);
    _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), memory, nullptr);
}

void Sahara::VulkanRenderer::copyImage(const QImage& image, VkImage vkImage)
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

VulkanUtil::UniformBuffers Sahara::VulkanRenderer::createArmatureUniformBuffers()
{
    return getUniformBuffers<AnimatedPipeline::Armature>(*_animatedPipeline, 4, 0);
}

VulkanUtil::UniformBuffers Sahara::VulkanRenderer::createMaterialUniformBuffers()
{
    return getUniformBuffers<ScenePipeline::Material>(*_scenePipeline, 2, 0);
}

VulkanUtil::UniformBuffers Sahara::VulkanRenderer::createLightingUniformBuffers()
{
    return getUniformBuffers<ScenePipeline::Lighting>(*_scenePipeline, 1, 0);
}

void Sahara::VulkanRenderer::destroyArmatureUniformBuffers(const VulkanUtil::UniformBuffers &buffers)
{
    _deviceFunctions->vkQueueWaitIdle(_vulkanWindow->graphicsQueue());
    _animatedPipeline->freeDescriptorSets(buffers.bufferDescriptorSets);
    for (int i = 0; i < buffers.buffers.size(); i++) {
        _deviceFunctions->vkUnmapMemory(_vulkanWindow->device(), buffers.buffersMemory[i]);
        _deviceFunctions->vkDestroyBuffer(_vulkanWindow->device(), buffers.buffers[i], nullptr);
        _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), buffers.buffersMemory[i], nullptr);
    }
}

void Sahara::VulkanRenderer::destroyMaterialUniformBuffers(const VulkanUtil::UniformBuffers &buffers)
{
    _deviceFunctions->vkQueueWaitIdle(_vulkanWindow->graphicsQueue());
    _scenePipeline->freeDescriptorSets(buffers.bufferDescriptorSets);
    for (int i = 0; i < buffers.buffers.size(); i++) {
        _deviceFunctions->vkUnmapMemory(_vulkanWindow->device(), buffers.buffersMemory[i]);
        _deviceFunctions->vkDestroyBuffer(_vulkanWindow->device(), buffers.buffers[i], nullptr);
        _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), buffers.buffersMemory[i], nullptr);
    }
}

void Sahara::VulkanRenderer::destroyLightingUniformBuffers(const VulkanUtil::UniformBuffers &buffers)
{
    _deviceFunctions->vkQueueWaitIdle(_vulkanWindow->graphicsQueue());
    _scenePipeline->freeDescriptorSets(buffers.bufferDescriptorSets);
    for (int i = 0; i < buffers.buffers.size(); i++) {
        _deviceFunctions->vkUnmapMemory(_vulkanWindow->device(), buffers.buffersMemory[i]);
        _deviceFunctions->vkDestroyBuffer(_vulkanWindow->device(), buffers.buffers[i], nullptr);
        _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), buffers.buffersMemory[i], nullptr);
    }
}

void Sahara::VulkanRenderer::releaseUniformBuffers(Pipeline& pipeline, const VulkanUtil::UniformBuffers &buffers)
{
    _deviceFunctions->vkQueueWaitIdle(_vulkanWindow->graphicsQueue());
    pipeline.freeDescriptorSets(buffers.bufferDescriptorSets);
    for (int i = 0; i < buffers.buffers.size(); i++) {
        _deviceFunctions->vkUnmapMemory(_vulkanWindow->device(), buffers.buffersMemory[i]);
        _deviceFunctions->vkDestroyBuffer(_vulkanWindow->device(), buffers.buffers[i], nullptr);
        _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), buffers.buffersMemory[i], nullptr);
    }
}

template <typename T>
VulkanUtil::UniformBuffers Sahara::VulkanRenderer::getUniformBuffers(Pipeline &pipeline, uint32_t set, uint32_t binding)
{
    int concurrentFrames = _vulkanWindow->concurrentFrameCount();
    VulkanUtil::UniformBuffers buffers;
    buffers.resize(concurrentFrames);

    for (int i = 0; i < concurrentFrames; i++) {
        VulkanUtil::createBuffer(_vulkanWindow, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, _vulkanWindow->hostVisibleMemoryIndex(), buffers.buffers[i], buffers.buffersMemory[i]);
        _deviceFunctions->vkMapMemory(_vulkanWindow->device(), buffers.buffersMemory[i], 0, sizeof(T), 0, &buffers.buffersMapped[i]);
    }
    buffers.bufferDescriptorSets = pipeline.createBufferDescriptorSets(set, binding, buffers.buffers, sizeof(T));

    return buffers;
}

void Sahara::VulkanRenderer::recordGrid(VulkanScene &scene)
{
    _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipelineWire->pipeline());
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipelineWire->pipelineLayout(), 0, 1, &_renderUniformBuffersGrid.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

    QList<VkBuffer> vertexBuffers = _grid->buffersByBinding(*_gridPipelineWire);
    QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
    _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());

    for (int i = 0; i < 2 * _grid->length(); i++) {
        _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 5, 1, 5 * i, 0);
    }

    if (showAxes()) {
        _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipeline->pipeline());
        _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _gridPipeline->pipelineLayout(), 0, 1, &_renderUniformBuffersGrid.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

        QList<VkBuffer> vertexBuffers = _grid->xAxis().buffersByBinding(*_gridPipeline);
        QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
        _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
        for (int i = 0; i < 6; i++) {
            _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 4, 1, 4 * i, 0);
        }

        vertexBuffers = _grid->yAxis().buffersByBinding(*_gridPipeline);
        offsets = QList<VkDeviceSize>(vertexBuffers.size(), 0);
        _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
        for (int i = 0; i < 6; i++) {
            _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 4, 1, 4 * i, 0);
        }

        vertexBuffers = _grid->zAxis().buffersByBinding(*_gridPipeline);
        offsets = QList<VkDeviceSize>(vertexBuffers.size(), 0);
        _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
        for (int i = 0; i < 6; i++) {
            _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), 4, 1, 4 * i, 0);
        }
    }
}

void Sahara::VulkanRenderer::recordDisplay(VulkanScene &scene, VulkanDisplay &display, const QMatrix4x4 &modelView, const bool focus)
{
    _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _displayPipeline->pipeline());
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _displayPipeline->pipelineLayout(), 0, 1, &_renderUniformBuffersDisplay.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

    float modelViewF[16];
    modelView.transposed().copyDataTo(modelViewF);
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _displayPipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(DisplayPipeline::PushConstants, modelView), sizeof(modelViewF), modelViewF);

    QVector3D cameraPosVec = scene.cameraNode().globalPosition();
    float cameraPositionF[] = {
        cameraPosVec.x(),
        cameraPosVec.y(),
        cameraPosVec.z()
    };
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _displayPipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(DisplayPipeline::PushConstants, cameraPosition), sizeof(cameraPositionF), cameraPositionF);

    int f = focus;
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _displayPipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(DisplayPipeline::PushConstants, focus), sizeof(f), &f);

    QList<VkBuffer> vertexBuffers = display.buffersByBinding(*_displayPipeline);
    QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
    _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());

    _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), display.vertexBuffers().first()->count(), 1, 0, 0);
}

void Sahara::VulkanRenderer::recordPointLight(VulkanScene &scene, const QMatrix4x4& modelView, const bool focus)
{
    recordDisplay(scene, *_pointLightDisplay, modelView, focus);
}

void Sahara::VulkanRenderer::recordCamera(VulkanScene &scene, const QMatrix4x4 &modelView, const bool focus)
{
    recordDisplay(scene, *_cameraDisplay, modelView, focus);
}

void Sahara::VulkanRenderer::recordScene(VulkanScene &scene, const float time)
{
    if (showGrid()) {
        recordGrid(scene);
    }

    scene.updateUniform(_vulkanWindow->currentFrame());

    QStack<QMatrix4x4> transforms;
    transforms.push(QMatrix4x4());
    scene.root().depthFirst([&](Node& node) {
       transforms.push(transforms.top() * node.transform());
       if (!node.isRoot()) {
           VulkanModel* model;
           PointLight* pointLight;
           Camera* camera;
           if ((model = dynamic_cast<VulkanModel*>(&node.item()))) {
               recordModel(*model, transforms, false, time);
               if (node.hasFocus()) {
                   recordModel(*model, transforms, true, time);
               }
           } else {
               if ((pointLight = dynamic_cast<PointLight*>(&node.item()))) {
                   if (showLights()) {
                       recordPointLight(scene, transforms.top(), node.hasFocus());
                   }
               } else if ((camera = dynamic_cast<Camera*>(&node.item()))) {
                   if (showCameras() && &node != &scene.cameraNode()) {
                       recordCamera(scene, transforms.top(), node.hasFocus());
                   }
               }
           }
       }

       return false;
    }, [&](Node&) {
        transforms.pop();
        return false;
     });
}

void Sahara::VulkanRenderer::recordSurface(Pipeline* pipeline, VulkanSurface &surface, Instance &instance, const bool focus)
{
    VulkanMaterial& material = dynamic_cast<VulkanMaterial&>(instance.getMaterial(surface.material()));

    material.updateUniform(_vulkanWindow->currentFrame());

    VkDescriptorSet descriptorSet = material.descriptorSets()[_vulkanWindow->currentFrame()];
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 2, 1, &descriptorSet, 0, nullptr);

    if (material.image().has_value()) {
        VkDescriptorSet descriptorSet = (*material.image())->descriptorSets()[_vulkanWindow->currentFrame()];
        _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 3, 1, &descriptorSet, 0, nullptr);
    } else {
        VkDescriptorSet descriptorSet = _emptyImage->descriptorSets()[_vulkanWindow->currentFrame()];
        _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 3, 1, &descriptorSet, 0, nullptr);
    }

    QList<VkBuffer> vertexBuffers = surface.buffersByBinding(*pipeline);

    // This is in case an unused vertex attribute is missing from the surface
    // i.e. bad data
    QList<QString> required = { "position", "normal", "texcoord", "joints", "weights" };
    QList<QString> have = surface.vertexBuffers().keys();

    for (int i = 0; i < required.size(); i++) {
        if (!have.contains(required[i])) {
            vertexBuffers.insert(i, _emptyBuffer);
        }
    }

    QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
    _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());

    _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), surface.vertexBuffers().first()->count(), 1, 0, 0);
}

void Sahara::VulkanRenderer::recordModel(VulkanModel &model, QStack<QMatrix4x4> &transformStack, const bool focus, const float time)
{
    model.animate(time);

    for (Instance* instance : model.instances()) {
        transformStack.push(transformStack.top() * instance->transform());

        float modelViewF[16];
        transformStack.top().transposed().copyDataTo(modelViewF);

        InstanceMesh* meshInstance;
        VulkanInstanceController* controllerInstance;
        if ((meshInstance = dynamic_cast<InstanceMesh*>(instance))) {
            Pipeline* pipeline = focus ? _scenePipelineWire : _scenePipeline;

            _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline());
            _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 0, 1, &_renderUniformBuffersScene.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), pipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(ScenePipeline::PushConstants, modelView), sizeof(modelViewF), modelViewF);

            VkDescriptorSet descriptorSet = _scene->descriptorSets()[_vulkanWindow->currentFrame()];
            _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 1, 1, &descriptorSet, 0, nullptr);

            float cp[3] = {
                _scene->cameraNode().globalPosition().x(),
                _scene->cameraNode().globalPosition().y(),
                _scene->cameraNode().globalPosition().z(),
            };
            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), pipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(ScenePipeline::PushConstants, cameraPosition), sizeof(cp), cp);

            int f = focus;
            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), pipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(ScenePipeline::PushConstants, focus), sizeof(f), &f);

            for (int i = 0; i < meshInstance->mesh().count(); i++) {
                recordSurface(pipeline, dynamic_cast<VulkanSurface&>(meshInstance->mesh().surface(i)), *meshInstance, focus);
            }
        } else if ((controllerInstance = dynamic_cast<VulkanInstanceController*>(instance))) {
            Pipeline* pipeline = focus ? _animatedPipelineWire : _animatedPipeline;

            _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline());
            _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 0, 1, &_renderUniformBuffersAnimated.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);

            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), pipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(AnimatedPipeline::PushConstants, modelView), sizeof(modelViewF), modelViewF);

            VkDescriptorSet descriptorSet = _scene->descriptorSets()[_vulkanWindow->currentFrame()];
            _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 1, 1, &descriptorSet, 0, nullptr);

            float cp[3] = {
                _scene->cameraNode().globalPosition().x(),
                _scene->cameraNode().globalPosition().y(),
                _scene->cameraNode().globalPosition().z(),
            };
            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), pipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(AnimatedPipeline::PushConstants, cameraPosition), sizeof(cp), cp);

            int f = focus;
            _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), pipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(AnimatedPipeline::PushConstants, focus), sizeof(f), &f);

            controllerInstance->updateUniform(_vulkanWindow->currentFrame());

            descriptorSet = controllerInstance->descriptorSets()[_vulkanWindow->currentFrame()];
            _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout(), 4, 1, &descriptorSet, 0, nullptr);

            for (int i = 0; i < controllerInstance->controller().mesh().count(); i++) {
                recordSurface(pipeline, dynamic_cast<VulkanSurface&>(controllerInstance->controller().mesh().surface(i)), *controllerInstance, focus);
            }
        }

        transformStack.pop();
    }
}

void Sahara::VulkanRenderer::recordPanel(Panel &panel)
{
    _deviceFunctions->vkCmdBindPipeline(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _panelPipeline->pipeline());
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _panelPipeline->pipelineLayout(), 0, 1, &_renderUniformBuffersPanel.bufferDescriptorSets[_vulkanWindow->currentFrame()], 0, nullptr);
    _deviceFunctions->vkCmdBindDescriptorSets(_vulkanWindow->currentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _panelPipeline->pipelineLayout(), 1, 1, &panel.descriptorSets()[_vulkanWindow->currentFrame()], 0, nullptr);

    float location[2] = { (float)panel.position().x(), (float)panel.position().y() };
    _deviceFunctions->vkCmdPushConstants(_vulkanWindow->currentCommandBuffer(), _panelPipeline->pipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offsetof(PanelPipeline::PushConstants, location), sizeof(location), location);

    panel.updateUniform(_vulkanWindow->currentFrame());

    QList<VkBuffer> vertexBuffers = panel.buffersByBinding(*_panelPipeline);
    QList<VkDeviceSize> offsets(vertexBuffers.size(), 0);
    _deviceFunctions->vkCmdBindVertexBuffers(_vulkanWindow->currentCommandBuffer(), 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
    _deviceFunctions->vkCmdDraw(_vulkanWindow->currentCommandBuffer(), panel.vertexBuffers().first()->count(), 1, 0, 0);
}

void Sahara::VulkanRenderer::record(const float time)
{
    // Update Render uniform used in all pipelines
    GridPipeline::Render render;
    _scene->cameraNode().globalTransform().inverted().transposed().copyDataTo(render.inverseCamera);
    (_vulkanWindow->clipCorrectionMatrix() * _scene->camera().projection()).transposed().copyDataTo(render.projection);
    memcpy(_renderUniformBuffersGrid.buffersMapped[_vulkanWindow->currentFrame()], &render, sizeof(GridPipeline::Render));
    memcpy(_renderUniformBuffersDisplay.buffersMapped[_vulkanWindow->currentFrame()], &render, sizeof(GridPipeline::Render));
    memcpy(_renderUniformBuffersScene.buffersMapped[_vulkanWindow->currentFrame()], &render, sizeof(GridPipeline::Render));
    memcpy(_renderUniformBuffersAnimated.buffersMapped[_vulkanWindow->currentFrame()], &render, sizeof(AnimatedPipeline::Render));

    recordScene(*_scene, time);

    if (_showFPS) {
        recordPanel(*_fpsPanel);
    }
}

void Sahara::VulkanRenderer::startNextFrame()
{
    if (_scene) {
        const QSize sz = _vulkanWindow->swapChainImageSize();

        VkClearColorValue clearColor = {{ 0.5f, 0.75f, 0.86f, 1.0f }};
        VkClearDepthStencilValue clearDS{
            .depth = 1.0f,
            .stencil = 0
        };
        VkClearValue clearValues[2];
        memset(clearValues, 0, sizeof(clearValues));
        clearValues[0].color = clearColor;
        clearValues[1].depthStencil = clearDS;

        VkRenderPassBeginInfo rpBeginInfo;
        memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
        rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBeginInfo.renderPass = _vulkanWindow->defaultRenderPass();
        rpBeginInfo.framebuffer = _vulkanWindow->currentFramebuffer();
        rpBeginInfo.renderArea.extent.width = sz.width();
        rpBeginInfo.renderArea.extent.height = sz.height();
        rpBeginInfo.clearValueCount = 2;
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

        record((float)_time.nsecsElapsed() / 1e9);

        _deviceFunctions->vkCmdEndRenderPass(cmdBuf);

        _vulkanWindow->frameReady();

        if (!_paused) {
            _vulkanWindow->requestUpdate();
        }
    }

    _fps = 1000.0 / _frameTime.restart();
}

void Sahara::VulkanRenderer::initSwapChainResources()
{
    GridPipeline::Render render;
    _scene->camera().setAspect((float)_vulkanWindow->width() / _vulkanWindow->height());
    _scene->cameraNode().globalTransform().inverted().transposed().copyDataTo(render.inverseCamera);
    (_vulkanWindow->clipCorrectionMatrix() * _scene->camera().projection()).transposed().copyDataTo(render.projection);

    // Panel
    _fpsPanel->setPosition(16, _vulkanWindow->height() - _fpsPanel->size().height() - 16);

    QMatrix4x4 proj(_vulkanWindow->clipCorrectionMatrix());
    proj.ortho(QRect(QPoint(), QSize(_vulkanWindow->width(), _vulkanWindow->height())));

    float projf[16];
    proj.transposed().copyDataTo(projf);

    for (int i = 0; i < _renderUniformBuffersGrid.buffers.size(); i++) {
        memcpy(_renderUniformBuffersGrid.buffersMapped[i], &render, sizeof(GridPipeline::Render));
        memcpy(_renderUniformBuffersDisplay.buffersMapped[i], &render, sizeof(GridPipeline::Render));
        memcpy(_renderUniformBuffersScene.buffersMapped[i], &render, sizeof(ScenePipeline::Render));
        memcpy(_renderUniformBuffersAnimated.buffersMapped[i], &render, sizeof(AnimatedPipeline::Render));
        memcpy(_renderUniformBuffersPanel.buffersMapped[i], projf, sizeof(projf));
    }
}

void Sahara::VulkanRenderer::initResources()
{
    _deviceFunctions = _vulkanWindow->vulkanInstance()->deviceFunctions(_vulkanWindow->device());

    _animatedPipeline = new AnimatedPipeline(_vulkanWindow, VK_POLYGON_MODE_FILL);
    _animatedPipelineWire = new AnimatedPipeline(_vulkanWindow, VK_POLYGON_MODE_LINE);
    _scenePipeline = new ScenePipeline(_vulkanWindow, VK_POLYGON_MODE_FILL);
    _scenePipelineWire = new ScenePipeline(_vulkanWindow, VK_POLYGON_MODE_LINE);
    _gridPipeline = new GridPipeline(_vulkanWindow, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
    _gridPipelineWire = new GridPipeline(_vulkanWindow, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);
    _displayPipeline = new DisplayPipeline(_vulkanWindow);
    _panelPipeline = new PanelPipeline(_vulkanWindow);

    _animatedPipeline->create();
    _animatedPipelineWire->create();
    _scenePipeline->create();
    _scenePipelineWire->create();
    _gridPipeline->create();
    _gridPipelineWire->create();
    _displayPipeline->create();
    _panelPipeline->create();

    _pointLightDisplay = new VulkanPointLightDisplay(_vulkanWindow);
    _cameraDisplay = new VulkanCameraDisplay(_vulkanWindow);
    _grid = new VulkanGrid(_vulkanWindow, 32);

    _renderUniformBuffersGrid = getUniformBuffers<GridPipeline::Render>(*_gridPipeline, 0, 0);
    _renderUniformBuffersDisplay = getUniformBuffers<DisplayPipeline::Render>(*_displayPipeline, 0, 0);
    _renderUniformBuffersScene = getUniformBuffers<ScenePipeline::Render>(*_scenePipeline, 0, 0);
    _renderUniformBuffersAnimated = getUniformBuffers<AnimatedPipeline::Render>(*_animatedPipeline, 0, 0);
    _renderUniformBuffersPanel = getUniformBuffers<PanelPipeline::Render>(*_panelPipeline, 0, 0);

    _fpsPanel = new FPSPanel(this);

    _emptyImage = new VulkanImage(this, "empty", "");
    VulkanUtil::createBuffer(_vulkanWindow, 1, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, _vulkanWindow->deviceLocalMemoryIndex(), _emptyBuffer, _emptyBufferMemory);

    _time.start();

    emit ready();
}

void Sahara::VulkanRenderer::releaseResources()
{
    _deviceFunctions->vkDestroyBuffer(_vulkanWindow->device(), _emptyBuffer, nullptr);
    _deviceFunctions->vkFreeMemory(_vulkanWindow->device(), _emptyBufferMemory, nullptr);

    delete _emptyImage;
    delete _fpsPanel;

    releaseUniformBuffers(*_panelPipeline, _renderUniformBuffersPanel);
    releaseUniformBuffers(*_animatedPipeline, _renderUniformBuffersAnimated);
    releaseUniformBuffers(*_scenePipeline, _renderUniformBuffersScene);
    releaseUniformBuffers(*_displayPipeline, _renderUniformBuffersDisplay);
    releaseUniformBuffers(*_gridPipeline, _renderUniformBuffersGrid);

    delete _grid;
    delete _cameraDisplay;
    delete _pointLightDisplay;
    delete _panelPipeline;
    delete _displayPipeline;
    delete _gridPipelineWire;
    delete _gridPipeline;
    delete _scenePipelineWire;
    delete _scenePipeline;
    delete _animatedPipelineWire;
    delete _animatedPipeline;
}
