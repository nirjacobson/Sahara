#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QKeyEvent>

#include "scene/scene.h"
#include "cameracontrol.h"
#include "render/renderer.h"

namespace Sahara {
    class SceneWidget
    {

    public:
        virtual Scene& scene() = 0;
        virtual void newScene() = 0;
        virtual void setScene(Scene* scene) = 0;
        virtual void flyThrough(const bool on) = 0;

        virtual void pause() = 0;
        virtual void resume() = 0;


        virtual bool showGrid() const = 0;
        virtual void showGrid(const bool visible) = 0;
        virtual bool showAxes() const = 0;
        virtual void showAxes(const bool visible) = 0;
        virtual bool showFPS() const = 0;
        virtual void showFPS(const bool visible) = 0;
        virtual bool showLights() const = 0;
        virtual void showLights(const bool visible) = 0;
        virtual bool showCameras() const = 0;
        virtual void showCameras(const bool visible) = 0;

        virtual Renderer* renderer() = 0;

        CameraControl& cameraControl();

    private:
        Scene* _scene;

        CameraControl _cameraControl;
    };
}

#endif // SCENEWIDGET_H
