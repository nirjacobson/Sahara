#ifndef RENDERER_H
#define RENDERER_H

#include <QStack>
#include <QMatrix4x4>

#include "../programs/sceneprogram.h"
#include "../programs/gridprogram.h"
#include "../programs/displayprogram.h"
#include "../scene/scene.h"
#include "../scene/model.h"
#include "../scene/asset/pointlight.h"
#include "vertexbuffer.h"
#include "grid/grid.h"
#include "../common/transform.h"
#include "display/pointlightdisplay.h"
#include "display/cameradisplay.h"

namespace Sahara {

    class Renderer
    {
        public:
            Renderer();
            ~Renderer();

            void render(Scene& scene, const float time);

            void showGrid(const bool visible);

        private:
            SceneProgram _sceneProgram;
            GridProgram _gridProgram;
            DisplayProgram _displayProgram;

            Grid _grid;
            bool _renderGrid;

            PointLightDisplay _pointLightDisplay;
            CameraDisplay _cameraDisplay;

            void renderScene(Scene& scene, const float time);
            void renderGrid(Scene& scene);
            void renderPointLight(Scene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderCamera(Scene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderModel(Model& model, const float time);
            void renderSurface(Surface& surface, Sahara::Instance& instance);

            void processSceneLighting(Scene& scene);
            void processControllerInstanceArmature(ControllerInstance& controllerInstance);
    };

}

#endif // RENDERER_H
