#include "scenewidget.h"

Sahara::SceneWidget::SceneWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    connect(&_timer, &QTimer::timeout, this, &SceneWidget::frame);
}

Sahara::SceneWidget::~SceneWidget()
{
    delete _renderer;
}

Sahara::Scene& Sahara::SceneWidget::scene()
{
    return _scene;
}

void Sahara::SceneWidget::showGrid(const bool visible)
{
    _renderer->renderGrid(visible);
}

void Sahara::SceneWidget::initializeGL()
{
    initializeOpenGLFunctions();

    _renderer = new Renderer;

    _timer.setInterval(1000 / 30);

    _time.start();
    _timer.start();

    emit initialized();
}

void Sahara::SceneWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    _renderer->render(_scene, _time.elapsed() / 1000.0f);

    double fps = 1000.0 / _frameTime.restart();

    QPainter painter(this);
    painter.drawText(16, height() - 16, QString::number(fps, 'f', 2)+" FPS");
}

void Sahara::SceneWidget::resizeGL(int w, int h)
{
    _scene.camera().setAspect(static_cast<float>(w) / h);
}

void Sahara::SceneWidget::frame()
{
    update();
}
