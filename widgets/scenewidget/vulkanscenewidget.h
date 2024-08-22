#ifndef SAHARA_VULKAN_SCENEWIDGET_H
#define SAHARA_VULKAN_SCENEWIDGET_H

#include "Sahara_global.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>

#include "vulkanscenewindow.h"
#include "cameracontrol.h"
#include "render/vulkanrenderer.h"
#include "scene/vulkanscene.h"
#include "scenewidget.h"

namespace Sahara {
    class SAHARA_EXPORT VulkanSceneWidget : public QWidget, public SceneWidget
    {
        Q_OBJECT
    public:
        explicit VulkanSceneWidget(QWidget *parent = nullptr);

        Scene& scene() override;
        void newScene() override;
        void setScene(Scene* scene) override;
        void flyThrough(const bool on) override;

        void pause() override;
        void resume() override;

        bool showGrid() const override;
        void showGrid(const bool visible) override;
        bool showAxes() const override;
        void showAxes(const bool visible) override;
        bool showFPS() const override;
        void showFPS(const bool visible) override;
        bool showLights() const override;
        void showLights(const bool visible) override;
        bool showCameras() const override;
        void showCameras(const bool visible) override;

        Renderer* renderer() override;

    signals:
        void initialized() const;
        void sizeChanged(QSize size);
        void cameraMotion() const;
        void keyPressed(QKeyEvent* event);
        void mouseMoved(QVector2D ndc);
        void mousePressed(QVector2D ndc);
        void sceneLoaded();

    private:
        VulkanScene* _scene;
        QVulkanInstance _instance;
        VulkanSceneWindow _window;
        VulkanRenderer* _renderer;

        QTimer _timer;

    private slots:
        void rendererCreated(QVulkanWindowRenderer* renderer);
        void rendererReady();
        void updateCameraControl();

        // QWidget interface
    protected:
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
    };
}

#endif // SAHARA_VULKAN_SCENEWIDGET_H
