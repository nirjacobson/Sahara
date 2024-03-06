#include "scenewindow.h"

Sahara::SceneWindow::SceneWindow()
{

}

QVulkanWindowRenderer* Sahara::SceneWindow::createRenderer()
{
    QVulkanWindowRenderer* renderer = new Renderer(this);
    emit rendererCreated(renderer);

    return renderer;
}
