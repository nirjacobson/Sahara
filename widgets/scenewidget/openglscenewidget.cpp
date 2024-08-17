#include "openglscenewidget.h"

Sahara::OpenGLSceneWidget::OpenGLSceneWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , _scene(new OpenGLScene)
    , _showFPS(true)
    , _flyThrough(false)
{
    connect(&_timer, SIGNAL(timeout(void)), this, SLOT(frame(void)));
    setMouseTracking(true);
}

Sahara::OpenGLSceneWidget::~OpenGLSceneWidget()
{
    makeCurrent();
    delete _renderer;
    delete _scene;
    doneCurrent();
}

Sahara::Scene& Sahara::OpenGLSceneWidget::scene()
{
    return *_scene;
}

void Sahara::OpenGLSceneWidget::newScene()
{
    delete _scene;
    _scene = new OpenGLScene;
    _scene->camera().setAspect(static_cast<float>(width()) / height());

    emit sceneLoaded();
}

void Sahara::OpenGLSceneWidget::setScene(Scene* scene)
{
    delete _scene;
    _scene = (OpenGLScene*)(scene);
    _scene->camera().setAspect(static_cast<float>(width()) / height());

    emit sceneLoaded();
}

void Sahara::OpenGLSceneWidget::flyThrough(const bool on)
{
    _flyThrough = on;
}

void Sahara::OpenGLSceneWidget::pause()
{
    _timer.stop();
}

void Sahara::OpenGLSceneWidget::resume()
{
    _timer.start();
}

bool Sahara::OpenGLSceneWidget::showGrid() const
{
    return _renderer->showGrid();
}

void Sahara::OpenGLSceneWidget::showGrid(const bool visible)
{
    _renderer->showGrid(visible);
}

bool Sahara::OpenGLSceneWidget::showAxes() const
{
    return _renderer->showAxes();
}

void Sahara::OpenGLSceneWidget::showAxes(const bool visible)
{
    _renderer->showAxes(visible);
}

bool Sahara::OpenGLSceneWidget::showFPS() const
{
    return _showFPS;
}

void Sahara::OpenGLSceneWidget::showFPS(const bool visible)
{
    _showFPS = visible;
}

bool Sahara::OpenGLSceneWidget::showLights() const
{
    return _renderer->showLights();
}

void Sahara::OpenGLSceneWidget::showLights(const bool visible)
{
    _renderer->showLights(visible);
}

bool Sahara::OpenGLSceneWidget::showCameras() const
{
    return _renderer->showCameras();
}

void Sahara::OpenGLSceneWidget::showCameras(const bool visible)
{
    _renderer->showCameras(visible);
}

Sahara::Renderer *Sahara::OpenGLSceneWidget::renderer()
{
    return _renderer;
}

void Sahara::OpenGLSceneWidget::initializeGL()
{
    initializeOpenGLFunctions();

    _renderer = new OpenGLRenderer;

    _timer.setInterval(1000 / 60);

    _time.start();
    _timer.start();

    emit initialized();
}

void Sahara::OpenGLSceneWidget::paintGL()
{
    static double fps = _fps;
    static int i = 0;
    i = (i + 1) % 15;

    glEnable(GL_DEPTH_TEST);
    _renderer->render(*_scene, _time.elapsed() / 1000.0f);
    _fps = 1000.0 / _frameTime.restart();

    if (i == 0) {
        fps = _fps;
    }

    if (_showFPS) {
        QPainter painter(this);
        painter.drawText(16, height() - 16, QString::number(fps, 'f', 0)+" FPS");
    }
}

void Sahara::OpenGLSceneWidget::resizeGL(int w, int h)
{
    _scene->camera().setAspect(static_cast<float>(w) / h);
    emit sizeChanged(QSize(w, h));
}

void Sahara::OpenGLSceneWidget::keyPressEvent(QKeyEvent* event)
{
    if (_flyThrough) {
        switch (event->key()) {
            case Qt::Key_W:
                cameraControl().accelerateForward(true);
                break;
            case Qt::Key_A:
                cameraControl().accelerateLeft(true);
                break;
            case Qt::Key_S:
                cameraControl().accelerateBackward(true);
                break;
            case Qt::Key_D:
                cameraControl().accelerateRight(true);
                break;
        }
    }

    emit keyPressed(event);

    QWidget::keyPressEvent(event);
}

void Sahara::OpenGLSceneWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (!_flyThrough)
        return;

    switch (event->key()) {
        case Qt::Key_W:
            cameraControl().accelerateForward(false);
            break;
        case Qt::Key_A:
            cameraControl().accelerateLeft(false);
            break;
        case Qt::Key_S:
            cameraControl().accelerateBackward(false);
            break;
        case Qt::Key_D:
            cameraControl().accelerateRight(false);
            break;
    }
}

void Sahara::OpenGLSceneWidget::mousePressEvent(QMouseEvent* event)
{
    QVector2D normalizedPos;
    normalizedPos.setX( 2 * (static_cast<float>(event->pos().x()) / width()) - 1 );
    normalizedPos.setY(1 - 2 * (static_cast<float>(event->pos().y()) / height()) );

    if (_flyThrough) {
        cameraControl().reset();
        _flyThrough = false;
    }

    emit mousePressed(normalizedPos);
}

void Sahara::OpenGLSceneWidget::mouseMoveEvent(QMouseEvent* event)
{
    QVector2D normalizedPos;
    normalizedPos.setX( 2 * (static_cast<float>(event->pos().x()) / width()) - 1 );
    normalizedPos.setY(1 - 2 * (static_cast<float>(event->pos().y()) / height()) );

    if (_flyThrough) {
        cameraControl().setRotationalVelocity(normalizedPos * 2);
    }

    emit mouseMoved(normalizedPos);
}

void Sahara::OpenGLSceneWidget::frame()
{
    if (_flyThrough) {
        emit cameraMotion();
    }
    update();
}
