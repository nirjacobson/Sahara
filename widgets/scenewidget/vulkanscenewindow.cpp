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
