#include "scenewindow.h"

Sahara::SceneWindow::SceneWindow(CameraControl& cameraControl)
    : _cameraControl(cameraControl)
    , _flyThrough(false)
{

}

QVulkanWindowRenderer* Sahara::SceneWindow::createRenderer()
{
    QVulkanWindowRenderer* renderer = new Renderer(this);

    emit rendererCreated(renderer);

    return renderer;
}

void Sahara::SceneWindow::flyThrough(const bool on)
{
    _flyThrough = on;
}

bool Sahara::SceneWindow::flyThrough() const
{
    return _flyThrough;
}

void Sahara::SceneWindow::mousePressEvent(QMouseEvent *event)
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

void Sahara::SceneWindow::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D normalizedPos;
    normalizedPos.setX( 2 * (static_cast<float>(event->pos().x()) / width()) - 1 );
    normalizedPos.setY(1 - 2 * (static_cast<float>(event->pos().y()) / height()) );

    if (_flyThrough) {
        _cameraControl.setRotationalVelocity(normalizedPos * 2);
    }

    emit mouseMoved(normalizedPos);
}
