#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <QStack>
#include <QMatrix4x4>
#include <QVulkanWindowRenderer>
#include <QVulkanDeviceFunctions>
#include <QElapsedTimer>

#include "../pipelines/animatedpipeline.h"
#include "../pipelines/scenepipeline.h"
#include "../pipelines/gridpipeline.h"
#include "../pipelines/displaypipeline.h"
#include "../pipelines/panelpipeline.h"
#include "../scene/asset/mesh/vulkansurface.h"
#include "grid/vulkangrid.h"
#include "display/vulkanpointlightdisplay.h"
#include "display/vulkancameradisplay.h"
#include "scene/instance/instancemesh.h"
#include "scene/instance/vulkaninstancecontroller.h"
#include "scene/asset/light/pointlight.h"
#include "vulkanutil.h"
#include "panel/fpspanel.h"
#include "scene/vulkanscene.h"
#include "renderer.h"

namespace Sahara {

    class VulkanRenderer : public Renderer, public QVulkanWindowRenderer
    {
        Q_OBJECT

        public:
            VulkanRenderer(QVulkanWindow* vulkanWindow);

            void setScene(VulkanScene* scene);

            bool showFPS() const;
            void showFPS(const bool visible);

            void pause();
            void resume();

            float fps() const;

            QVulkanWindow* window();

            void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage &image, VkDeviceMemory &imageMemory);
            VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
            QList<VkDescriptorSet> createImageDescriptorSets(VkImageView imageView);
            void freeImageDescriptorSets(const QList<VkDescriptorSet>& descriptorSets);
            void freePanelImageDescriptorSets(const QList<VkDescriptorSet>& descriptorSets);

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
            PanelPipeline* _panelPipeline;

            VulkanImage* _emptyImage;
            VkBuffer _emptyBuffer;
            VkDeviceMemory _emptyBufferMemory;

            QElapsedTimer _frameTime;
            float _fps;

            QElapsedTimer _time;

            VulkanScene* _scene;

            bool _paused;

            VulkanGrid* _grid;
            bool _showFPS;

            FPSPanel* _fpsPanel;

            VulkanPointLightDisplay* _pointLightDisplay;
            VulkanCameraDisplay* _cameraDisplay;

            template <typename T>
            VulkanUtil::UniformBuffers getUniformBuffers(Pipeline& pipeline, uint32_t set, uint32_t binding);
            void releaseUniformBuffers(Pipeline &pipeline, const VulkanUtil::UniformBuffers& buffers);

            VulkanUtil::UniformBuffers _renderUniformBuffersGrid;
            VulkanUtil::UniformBuffers _renderUniformBuffersDisplay;
            VulkanUtil::UniformBuffers _renderUniformBuffersScene;
            VulkanUtil::UniformBuffers _renderUniformBuffersAnimated;
            VulkanUtil::UniformBuffers _renderUniformBuffersPanel;

            void recordGrid(VulkanScene& scene);
            void recordDisplay(VulkanScene& scene, VulkanDisplay& display, const QMatrix4x4& modelView, const bool focus);
            void recordPointLight(VulkanScene& scene, const QMatrix4x4 &modelView, const bool focus);
            void recordCamera(VulkanScene& scene, const QMatrix4x4 &modelView, const bool focus);
            void recordScene(VulkanScene& scene, const float time);
            void recordSurface(Pipeline *pipeline, Sahara::VulkanSurface& surface, Instance& instance, const bool focus);
            void recordModel(Sahara::VulkanModel& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time);
            void recordPanel(Panel& panel);

            void record(const float time);

            // QVulkanWindowRenderer interface
        public:
            void startNextFrame();
            void initSwapChainResources();
            void initResources();
            void releaseResources();
    };

}

#endif // VULKANRENDERER_H
