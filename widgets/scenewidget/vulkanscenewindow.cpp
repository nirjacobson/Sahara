#include "vulkanscenewindow.h"

Sahara::VulkanSceneWindow::VulkanSceneWindow(CameraControl& cameraControl)
    : _cameraControl(cameraControl)
    , _flyThrough(false)
{

}

QVulkanWindowRenderer* Sahara::VulkanSceneWindow::createRenderer()
{
    QVulkanWindowRenderer* renderer = new VulkanRenderer(this);

    emit rendererCreated(renderer);

    return renderer;
}

void Sahara::VulkanSceneWindow::flyThrough(const bool on)
{
    _flyThrough = on;
}

bool Sahara::VulkanSceneWindow::flyThrough() const
{
    return _flyThrough;
}

void Sahara::VulkanSceneWindow::mousePressEvent(QMouseEvent *event)
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

void Sahara::VulkanSceneWindow::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D normalizedPos;
    normalizedPos.setX( 2 * (static_cast<float>(event->pos().x()) / width()) - 1 );
    normalizedPos.setY(1 - 2 * (static_cast<float>(event->pos().y()) / height()) );

    if (_flyThrough) {
        _cameraControl.setRotationalVelocity(normalizedPos * 2);
    }

    emit mouseMoved(normalizedPos);
}

void Sahara::VulkanSceneWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    if (flyThrough()) {
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

    QWindow::keyPressEvent(event);
}

void Sahara::VulkanSceneWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    if (!flyThrough())
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

    QWindow::keyReleaseEvent(event);
}
