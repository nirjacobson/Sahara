#include "scenewidget.h"

Sahara::Scene& Sahara::SceneWidget::scene()
{
    return *_scene;
}

Sahara::CameraControl& Sahara::SceneWidget::cameraControl()
{
    return _cameraControl;
}
