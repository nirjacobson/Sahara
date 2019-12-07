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

namespace Sahara {

    class SceneWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT

        public:
            SceneWidget(QWidget* parent);
            ~SceneWidget();

            Scene& scene();

        signals:
            void initialized() const;

        protected:
            void initializeGL() override;
            void paintGL() override;
            void resizeGL(int w, int h) override;

        private:
            Sahara::Scene _scene;
            Sahara::Renderer* _renderer;

            QTime _time;
            QTime _frameTime;
            QTimer _timer;

        private slots:
            void frame();
    };
}

#endif // SAHARA_SCENEWIDGET_H
