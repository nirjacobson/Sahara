#include "vulkanscenewidget.h"

Sahara::VulkanSceneWidget::VulkanSceneWidget(QWidget *parent)
    : QWidget{parent}
    , _scene(nullptr)
    , _renderer(nullptr)
    , _window(cameraControl())
{
    _instance.setLayers({ "VK_LAYER_KHRONOS_validation" });
    if (!_instance.create())
        qFatal("Failed to create Vulkan instance: %d", _instance.errorCode());

    _window.setVulkanInstance(&_instance);

    connect(&_window, SIGNAL(rendererCreated(QVulkanWindowRenderer*)), this, SLOT(rendererCreated(QVulkanWindowRenderer*)));
    connect(&_window, SIGNAL(keyPressed(QKeyEvent*)), this, SIGNAL(keyPressed(QKeyEvent*)));
    connect(&_window, SIGNAL(mouseMoved(QVector2D)), this, SIGNAL(mouseMoved(QVector2D)));
    connect(&_window, SIGNAL(mousePressed(QVector2D)), this, SIGNAL(mousePressed(QVector2D)));

    QWidget *wrapper = QWidget::createWindowContainer(&_window);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(wrapper);
    setLayout(layout);

    connect(&_timer, SIGNAL(timeout(void)), this, SLOT(updateCameraControl(void)));
    _timer.setInterval(1000 / 60);
}

Sahara::Scene &Sahara::VulkanSceneWidget::scene()
{
    return *_scene;
}

void Sahara::VulkanSceneWidget::newScene()
{
    _scene = new VulkanScene(_renderer);
    _renderer->setScene(_scene);
}

void Sahara::VulkanSceneWidget::rendererCreated(QVulkanWindowRenderer* renderer)
{
    _renderer = dynamic_cast<VulkanRenderer*>(renderer);
    connect((Renderer*)_renderer, SIGNAL(ready(void)), this, SLOT(rendererReady(void)));
}

void Sahara::VulkanSceneWidget::rendererReady()
{
    if (!_scene) {
        newScene();

        _timer.start();

        emit initialized();
    } else {
        _scene->recreateUniforms();
    }
}

void Sahara::VulkanSceneWidget::updateCameraControl()
{
    if (_window.flyThrough()) {
        cameraControl().update(_scene->cameraNode());
        emit cameraMotion();
    }
}

void Sahara::VulkanSceneWidget::keyPressEvent(QKeyEvent *event)
{
#ifndef Q_OS_WIN
    if (event->isAutoRepeat())
        return;

    if (_window.flyThrough()) {
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
#endif

    QWidget::keyPressEvent(event);
}

void Sahara::VulkanSceneWidget::keyReleaseEvent(QKeyEvent *event)
{
#ifndef Q_OS_WIN
    if (event->isAutoRepeat())
        return;

    if (!_window.flyThrough())
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
#endif

    QWidget::keyReleaseEvent(event);
}

void Sahara::VulkanSceneWidget::setScene(Scene* scene)
{
    VulkanScene* s = _scene;
    _scene = (VulkanScene*)scene;
    _renderer->setScene(_scene);

    delete s;

    _scene->camera().setAspect(static_cast<float>(width()) / height());

    emit sceneLoaded();
}

void Sahara::VulkanSceneWidget::flyThrough(const bool on)
{
    _window.setKeyboardGrabEnabled(true);
    _window.flyThrough(on);
}

void Sahara::VulkanSceneWidget::pause()
{
    _renderer->pause();
}

void Sahara::VulkanSceneWidget::resume()
{
    _renderer->resume();
}

bool Sahara::VulkanSceneWidget::showGrid() const
{
    return _renderer->showGrid();
}

void Sahara::VulkanSceneWidget::showGrid(const bool visible)
{
    _renderer->showGrid(visible);
}

bool Sahara::VulkanSceneWidget::showAxes() const
{
    return _renderer->showAxes();
}

void Sahara::VulkanSceneWidget::showAxes(const bool visible)
{
    _renderer->showAxes(visible);
}

bool Sahara::VulkanSceneWidget::showFPS() const
{
    return _renderer->showFPS();
}

void Sahara::VulkanSceneWidget::showFPS(const bool visible)
{
    _renderer->showFPS(visible);
}

bool Sahara::VulkanSceneWidget::showLights() const
{
    return _renderer->showLights();
}

void Sahara::VulkanSceneWidget::showLights(const bool visible)
{
    _renderer->showLights(visible);
}

bool Sahara::VulkanSceneWidget::showCameras() const
{
    return _renderer->showCameras();
}

void Sahara::VulkanSceneWidget::showCameras(const bool visible)
{
    _renderer->showCameras(visible);
}

Sahara::Renderer *Sahara::VulkanSceneWidget::renderer()
{
    return _renderer;
}

