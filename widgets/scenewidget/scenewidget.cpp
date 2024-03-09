#include "scenewidget.h"

Sahara::SceneWidget::SceneWidget(QWidget *parent)
    : QWidget{parent}
    , _scene(nullptr)
    , _renderer(nullptr)
    , _window(_cameraControl)
{
    _instance.setLayers({ "VK_LAYER_KHRONOS_validation" });
    if (!_instance.create())
        qFatal("Failed to create Vulkan instance: %d", _instance.errorCode());

    _window.setVulkanInstance(&_instance);

    connect(&_window, &SceneWindow::rendererCreated, this, &SceneWidget::rendererCreated);
    connect(&_window, &SceneWindow::keyPressed, this, &SceneWidget::keyPressed);
    connect(&_window, &SceneWindow::mouseMoved, this, &SceneWidget::mouseMoved);
    connect(&_window, &SceneWindow::mousePressed, this, &SceneWidget::mousePressed);

    QWidget *wrapper = QWidget::createWindowContainer(&_window);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(wrapper);
    setLayout(layout);

    connect(&_timer, &QTimer::timeout, this, &SceneWidget::updateCameraControl);
    _timer.setInterval(1000 / 60);
}

Sahara::SceneWidget::~SceneWidget()
{

}

Sahara::Scene &Sahara::SceneWidget::scene()
{
    return *_scene;
}

void Sahara::SceneWidget::newScene()
{
    _scene = new Scene(_renderer);
    _renderer->setScene(_scene);
}

void Sahara::SceneWidget::rendererCreated(QVulkanWindowRenderer* renderer)
{
    _renderer = dynamic_cast<Renderer*>(renderer);
    connect(_renderer, &Renderer::ready, this, &SceneWidget::rendererReady);
}

void Sahara::SceneWidget::rendererReady()
{
    newScene();

    _timer.start();

    emit initialized();
}

void Sahara::SceneWidget::updateCameraControl()
{
    if (_window.flyThrough()) {
        _cameraControl.update(_scene->cameraNode());
        emit cameraMotion();
    }
}

void Sahara::SceneWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    if (_window.flyThrough()) {
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

void Sahara::SceneWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    if (!_window.flyThrough())
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

void Sahara::SceneWidget::setScene(Sahara::Scene* scene)
{
    Scene* s = _scene;
    _scene = scene;
    _renderer->setScene(_scene);

    delete s;

    _scene->camera().setAspect(static_cast<float>(width()) / height());

    emit sceneLoaded();
}

void Sahara::SceneWidget::flyThrough(const bool on)
{
    _window.setKeyboardGrabEnabled(true);
    _window.flyThrough(on);
}

void Sahara::SceneWidget::pause()
{
    _renderer->pause();
}

void Sahara::SceneWidget::resume()
{
    _renderer->resume();
}

bool Sahara::SceneWidget::showGrid() const
{
    return _renderer->showGrid();
}

void Sahara::SceneWidget::showGrid(const bool visible)
{
    _renderer->showGrid(visible);
}

bool Sahara::SceneWidget::showAxes() const
{
    return _renderer->showAxes();
}

void Sahara::SceneWidget::showAxes(const bool visible)
{
    _renderer->showAxes(visible);
}

bool Sahara::SceneWidget::showFPS() const
{
    return _showFPS;
}

void Sahara::SceneWidget::showFPS(const bool visible)
{
    _showFPS = visible;
}

bool Sahara::SceneWidget::showLights() const
{
    return _renderer->showLights();
}

void Sahara::SceneWidget::showLights(const bool visible)
{
    _renderer->showLights(visible);
}

bool Sahara::SceneWidget::showCameras() const
{
    return _renderer->showCameras();
}

void Sahara::SceneWidget::showCameras(const bool visible)
{
    _renderer->showCameras(visible);
}

Sahara::Renderer *Sahara::SceneWidget::renderer()
{
    return _renderer;
}
