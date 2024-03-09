#ifndef RENDERER_H
#define RENDERER_H

#include <QStack>
#include <QMatrix4x4>
#include <QVulkanWindowRenderer>
#include <QVulkanDeviceFunctions>
#include <QElapsedTimer>

#include "../pipelines/animatedpipeline.h"
#include "../pipelines/scenepipeline.h"
#include "../pipelines/gridpipeline.h"
#include "../pipelines/displaypipeline.h"
#include "../scene/scene.h"
#include "../scene/model.h"
#include "../scene/asset/mesh/surface.h"
#include "grid/grid.h"
#include "display/pointlightdisplay.h"
#include "display/cameradisplay.h"
#include "scene/instance/instancemesh.h"
#include "scene/instance/instancecontroller.h"
#include "scene/asset/light/pointlight.h"
#include "vulkanutil.h"

namespace Sahara {

    class Renderer : public QObject, public QVulkanWindowRenderer
    {
        Q_OBJECT

        public:
            Renderer(QVulkanWindow* vulkanWindow);

            // void render(const float time);

            void setScene(Scene* scene);

            bool showGrid() const;
            void showGrid(const bool visible);
            bool showAxes() const;
            void showAxes(const bool visible);
            bool showLights() const;
            void showLights(const bool visible);
            bool showCameras() const;
            void showCameras(const bool visible);

            void pause();
            void resume();

            float fps() const;

            QVulkanWindow* window();

            void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage &image, VkDeviceMemory &imageMemory);
            VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
            QList<VkDescriptorSet> createImageDescriptorSets(VkImageView imageView);
            void freeImageDescriptorSets(const QList<VkDescriptorSet>& descriptorSets);

            void destroyImage(VkImage image, VkDeviceMemory memory, VkImageView imageView);

            void copyImage(const QImage& image, VkImage vkImage);

            VulkanUtil::UniformBuffers createArmatureUniformBuffers();
            VulkanUtil::UniformBuffers createMaterialUniformBuffers();
            VulkanUtil::UniformBuffers createLightingUniformBuffers();
            void destroyArmatureUniformBuffers(const VulkanUtil::UniformBuffers& buffers);
            void destroyMaterialUniformBuffers(const VulkanUtil::UniformBuffers& buffers);
            void destroyLightingUniformBuffers(const VulkanUtil::UniformBuffers& buffers);

        signals:
            void ready();

        private:
            QVulkanWindow* _vulkanWindow;
            QVulkanDeviceFunctions* _deviceFunctions;

            AnimatedPipeline* _animatedPipeline;
            AnimatedPipeline* _animatedPipelineWire;
            ScenePipeline* _scenePipeline;
            ScenePipeline* _scenePipelineWire;
            GridPipeline* _gridPipeline;
            GridPipeline* _gridPipelineWire;
            DisplayPipeline* _displayPipeline;

            Image* _emptyImage;
            VkBuffer _emptyBuffer;
            VkDeviceMemory _emptyBufferMemory;

            QElapsedTimer _frameTime;
            float _fps;

            QElapsedTimer _time;

            Scene* _scene;

            bool _paused;

            Grid* _grid;
            bool _showGrid;
            bool _showAxes;
            bool _showLights;
            bool _showCameras;

            PointLightDisplay* _pointLightDisplay;
            CameraDisplay* _cameraDisplay;

            template <typename T>
            VulkanUtil::UniformBuffers getUniformBuffers(Pipeline& pipeline, uint32_t set, uint32_t binding);
            void releaseUniformBuffers(Pipeline &pipeline, const VulkanUtil::UniformBuffers& buffers);

            void renderScene(Scene& scene, const float time);
            void renderGrid(Scene& scene);
            void renderPointLight(Scene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderCamera(Scene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderModel(Model& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time);
            void renderSurface(Surface& surface, Sahara::Instance& instance, const bool focus);

            void processSceneLighting(Scene& scene);
            void processControllerInstanceArmature(InstanceController& controllerInstance);

            VulkanUtil::UniformBuffers _renderUniformBuffersGrid;
            VulkanUtil::UniformBuffers _renderUniformBuffersDisplay;
            VulkanUtil::UniformBuffers _renderUniformBuffersScene;
            VulkanUtil::UniformBuffers _renderUniformBuffersAnimated;

            void recordGrid(Scene& scene);
            void recordDisplay(Scene& scene, Display& display, const QMatrix4x4& modelView, const bool focus);
            void recordPointLight(Scene& scene, const QMatrix4x4 &modelView, const bool focus);
            void recordCamera(Scene& scene, const QMatrix4x4 &modelView, const bool focus);
            void recordScene(Scene& scene, const float time);
            void recordSurface(Pipeline *pipeline, Sahara::Surface& surface, Instance& instance, const bool focus);
            void recordModel(Sahara::Model& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time);

            void record(const float time);
            // QVulkanWindowRenderer interface
        public:
            void startNextFrame();
            void initSwapChainResources();
            void initResources();
            void releaseResources();
    };

}

#endif // RENDERER_H
