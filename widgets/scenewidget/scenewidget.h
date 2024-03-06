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

    protected:
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

    private:
        Scene* _scene;
        QVulkanInstance _instance;
        SceneWindow _window;
        Renderer* _renderer;

        bool _showFPS;

        bool _flyThrough;
        CameraControl _cameraControl;

        double _fps;

        QTimer _timer;

    private slots:
        void rendererCreated(QVulkanWindowRenderer* renderer);
        void updateCameraControl();

    signals:

        // QWidget interface
    protected:
        void paintEvent(QPaintEvent *event) override;
    };
}

#endif // SAHARA_SCENEWIDGET_H
