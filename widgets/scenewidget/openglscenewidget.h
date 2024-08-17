#ifndef SAHARA_OPENGL_SCENEWIDGET_H
#define SAHARA_OPENGL_SCENEWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QTime>
#include <QTimer>
#include <QPainter>

#include "scene/openglscene.h"
#include "render/openglrenderer.h"
#include "cameracontrol.h"
#include "scenewidget.h"

namespace Sahara {

    class OpenGLSceneWidget :  public QOpenGLWidget, protected QOpenGLFunctions, public SceneWidget
    {
        Q_OBJECT

        public:
            OpenGLSceneWidget(QWidget* parent);
            ~OpenGLSceneWidget() override;

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

        protected:
            void initializeGL() override;
            void paintGL() override;
            void resizeGL(int w, int h) override;
            void keyPressEvent(QKeyEvent* event) override;
            void keyReleaseEvent(QKeyEvent* event) override;
            void mousePressEvent(QMouseEvent* event) override;
            void mouseMoveEvent(QMouseEvent* event) override;

        private:
            Sahara::OpenGLScene* _scene;
            Sahara::OpenGLRenderer* _renderer;

            bool _showFPS;

            bool _flyThrough;

            QElapsedTimer _time;
            QElapsedTimer _frameTime;
            QTimer _timer;

            double _fps;

        private slots:
            void frame();
    };
}

#endif // SAHARA_OPENGL_SCENEWIDGET_H
