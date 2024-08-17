#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QStack>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

#include "programs/sceneprogram.h"
#include "programs/gridprogram.h"
#include "programs/displayprogram.h"
#include "scene/openglscene.h"
#include "scene/openglmodel.h"
#include "scene/asset/mesh/openglsurface.h"
#include "grid/openglgrid.h"
#include "display/openglpointlightdisplay.h"
#include "display/openglcameradisplay.h"
#include "scene/instance/instancemesh.h"
#include "scene/instance/openglinstancecontroller.h"
#include "renderer.h"

namespace Sahara {

    class OpenGLRenderer : public Renderer
    {
        public:
            OpenGLRenderer();
            ~OpenGLRenderer();

            void render(OpenGLScene& scene, const float time);

        private:
            SceneProgram _sceneProgram;
            GridProgram _gridProgram;
            DisplayProgram _displayProgram;

            OpenGLGrid _grid;
            OpenGLPointLightDisplay _pointLightDisplay;
            OpenGLCameraDisplay _cameraDisplay;

            void renderScene(OpenGLScene& scene, const float time);
            void renderGrid(OpenGLScene& scene);
            void renderPointLight(OpenGLScene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderCamera(OpenGLScene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderModel(OpenGLModel& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time);
            void renderSurface(OpenGLSurface& surface, Sahara::Instance& instance, const bool focus);

            void processSceneLighting(OpenGLScene& scene);
            void processControllerInstanceArmature(OpenGLInstanceController& controllerInstance);
    };

}

#endif // OPENGLRENDERER_H
