#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QVulkanWindow>

#include "render/renderer.h"

namespace Sahara {
    class SceneWindow : public QVulkanWindow
    {
        Q_OBJECT

    public:
        SceneWindow();

        QVulkanWindowRenderer* createRenderer();

    signals:
        void rendererCreated(QVulkanWindowRenderer* renderer);
    };
}

#endif // SCENEWINDOW_H
