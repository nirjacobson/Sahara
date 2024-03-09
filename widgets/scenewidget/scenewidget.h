#ifndef SAHARA_SCENEWIDGET_H
#define SAHARA_SCENEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>

#include "scenewindow.h"
#include "cameracontrol.h"

namespace Sahara {
    class SceneWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit SceneWidget(QWidget *parent = nullptr);
        ~SceneWidget();

        Scene& scene();
        void newScene();
        void setScene(Scene* scene);
        void flyThrough(const bool on);

        void pause();
        void resume();

        bool showGrid() const;
        void showGrid(const bool visible);
        bool showAxes() const;
        void showAxes(const bool visible);
        bool showFPS() const;
        void showFPS(const bool visible);
        bool showLights() const;
        void showLights(const bool visible);
        bool showCameras() const;
        void showCameras(const bool visible);

        Renderer* renderer();

    signals:
        void initialized() const;
        void sizeChanged(QSize size);
        void cameraMotion() const;
        void keyPressed(QKeyEvent* event);
        void mouseMoved(QVector2D ndc);
        void mousePressed(QVector2D ndc);
        void sceneLoaded();

    private:
        Scene* _scene;
        QVulkanInstance _instance;
        SceneWindow _window;
        Renderer* _renderer;

        CameraControl _cameraControl;

        QTimer _timer;

    private slots:
        void rendererCreated(QVulkanWindowRenderer* renderer);
        void rendererReady();
        void updateCameraControl();

    signals:

        // QWidget interface
    protected:
        void keyPressEvent(QKeyEvent *event);

        // QWidget interface
    protected:
        void keyReleaseEvent(QKeyEvent *event);
    };
}

#endif // SAHARA_SCENEWIDGET_H
