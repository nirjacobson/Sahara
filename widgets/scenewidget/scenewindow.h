#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QVulkanWindow>
#include <QKeyEvent>

#include "render/renderer.h"
#include "widgets/scenewidget/cameracontrol.h"

namespace Sahara {
    class SceneWindow : public QVulkanWindow
    {
        Q_OBJECT

    public:
        SceneWindow(CameraControl& cameraControl);

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
    };
}

#endif // SCENEWINDOW_H
