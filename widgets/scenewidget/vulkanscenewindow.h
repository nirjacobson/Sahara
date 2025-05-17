#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QVulkanWindow>
#include <QKeyEvent>
#include <QWidget>

#include "../../render/vulkanrenderer.h"
#include "cameracontrol.h"

namespace Sahara {
    class VulkanSceneWindow : public QVulkanWindow
    {
        Q_OBJECT

    public:
        VulkanSceneWindow(CameraControl& cameraControl);

        QVulkanWindowRenderer* createRenderer();

        void flyThrough(const bool on);
        bool flyThrough() const;

    signals:
        void rendererCreated(QVulkanWindowRenderer* renderer);
        void keyPressed(QKeyEvent* event);
        void mouseMoved(QVector2D ndc);
        void mousePressed(QVector2D ndc);

    private:
        CameraControl& _cameraControl;
        bool _flyThrough;

        // QWindow interface
    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);
    };
}

#endif // SCENEWINDOW_H
