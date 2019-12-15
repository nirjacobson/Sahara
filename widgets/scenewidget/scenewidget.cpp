#include "scenewidget.h"

Sahara::SceneWidget::SceneWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , _scene(new Scene)
    , _flyThrough(false)
{
    connect(&_timer, &QTimer::timeout, this, &SceneWidget::frame);
    setMouseTracking(true);
}

Sahara::SceneWidget::~SceneWidget()
{
    delete _renderer;
    delete _scene;
}

Sahara::Scene& Sahara::SceneWidget::scene()
{
    return *_scene;
}

void Sahara::SceneWidget::newScene()
{
    delete _scene;
    _scene = new Scene;
}

void Sahara::SceneWidget::setScene(Sahara::Scene* scene)
{
    delete _scene;
    _scene = scene;
}

void Sahara::SceneWidget::flyThrough(const bool on)
{
    _flyThrough = on;
}

void Sahara::SceneWidget::showGrid(const bool visible)
{
    _renderer->showGrid(visible);
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
    _renderer->render(*_scene, _time.elapsed() / 1000.0f);

    double fps = 1000.0 / _frameTime.restart();

    QPainter painter(this);
    painter.drawText(16, height() - 16, QString::number(fps, 'f', 2)+" FPS");
}

void Sahara::SceneWidget::resizeGL(int w, int h)
{
    _scene->camera().setAspect(static_cast<float>(w) / h);
    emit sizeChanged(QSize(w, h));
}

void Sahara::SceneWidget::keyPressEvent(QKeyEvent* event)
{
    if (_flyThrough) {
        switch (event->key()) {
            case Qt::Key_W:
                _cameraControl.accelerateForward(true);
                break;
            case Qt::Key_A:
                _cameraControl.accelerateLeft(true);
                break;
            case Qt::Key_S:
                _cameraControl.accelerateBackward(true);
                break;
            case Qt::Key_D:
                _cameraControl.accelerateRight(true);
                break;
        }
    }

    emit keyPressed(event);

    QWidget::keyPressEvent(event);
}

void Sahara::SceneWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (!_flyThrough)
        return;

    switch (event->key()) {
        case Qt::Key_W:
            _cameraControl.accelerateForward(false);
            break;
        case Qt::Key_A:
            _cameraControl.accelerateLeft(false);
            break;
        case Qt::Key_S:
            _cameraControl.accelerateBackward(false);
            break;
        case Qt::Key_D:
            _cameraControl.accelerateRight(false);
            break;
    }
}

void Sahara::SceneWidget::mousePressEvent(QMouseEvent* event)
{
    QVector2D normalizedPos;
    normalizedPos.setX( 2 * (static_cast<float>(event->pos().x()) / width()) - 1 );
    normalizedPos.setY(1 - 2 * (static_cast<float>(event->pos().y()) / height()) );

    if (_flyThrough) {
        _cameraControl.reset();
        _flyThrough = false;
    }

    emit mousePressed(normalizedPos);
}

void Sahara::SceneWidget::mouseMoveEvent(QMouseEvent* event)
{
    QVector2D normalizedPos;
    normalizedPos.setX( 2 * (static_cast<float>(event->pos().x()) / width()) - 1 );
    normalizedPos.setY(1 - 2 * (static_cast<float>(event->pos().y()) / height()) );

    if (_flyThrough) {
        _cameraControl.setRotationalVelocity(normalizedPos * 2);
    }

    emit mouseMoved(normalizedPos);
}

void Sahara::SceneWidget::frame()
{
    if (_flyThrough) {
        _cameraControl.update(_scene->cameraNode());
        emit cameraMotion();
    }
    update();
}
