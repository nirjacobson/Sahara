#ifndef SAHARA_SCENEWIDGET_H
#define SAHARA_SCENEWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTime>
#include <QTimer>
#include <QPainter>

#include "scene/scene.h"
#include "render/renderer.h"
#include "cameracontrol.h"

namespace Sahara {

    class SceneWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT

        public:
            SceneWidget(QWidget* parent);
            ~SceneWidget() override;

            Scene& scene();
            void newScene();
            void setScene(Scene* scene);
            void flyThrough(const bool on);

            void showGrid(const bool visible);

        signals:
            void initialized() const;
            void sizeChanged(QSize size);
            void cameraMotion() const;
            void keyPressed(QKeyEvent* event);
            void mouseMoved(QVector2D ndc);
            void mousePressed(QVector2D ndc);
            void sceneLoaded();

        protected:
            void initializeGL() override;
            void paintGL() override;
            void resizeGL(int w, int h) override;
            void keyPressEvent(QKeyEvent* event) override;
            void keyReleaseEvent(QKeyEvent* event) override;
            void mousePressEvent(QMouseEvent* event) override;
            void mouseMoveEvent(QMouseEvent* event) override;

        private:
            Sahara::Scene* _scene;
            Sahara::Renderer* _renderer;

            bool _flyThrough;
            CameraControl _cameraControl;

            QTime _time;
            QTime _frameTime;
            QTimer _timer;

        private slots:
            void frame();
    };
}

#endif // SAHARA_SCENEWIDGET_H
