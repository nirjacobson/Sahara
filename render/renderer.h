#ifndef RENDERER_H
#define RENDERER_H

#include <QStack>
#include <QMatrix4x4>

#include "../programs/sceneprogram.h"
#include "../programs/gridprogram.h"
#include "../programs/displayprogram.h"
#include "../scene/scene.h"
#include "../scene/model.h"
#include "../scene/asset/mesh/mesh.h"
#include "../scene/asset/mesh/surface.h"
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

            bool showGrid() const;
            void showGrid(const bool visible);
            bool showAxes() const;
            void showAxes(const bool visible);

        private:
            SceneProgram _sceneProgram;
            GridProgram _gridProgram;
            DisplayProgram _displayProgram;

            Grid _grid;
            bool _showGrid;
            bool _showAxes;

            PointLightDisplay _pointLightDisplay;
            CameraDisplay _cameraDisplay;

            void renderScene(Scene& scene, const float time);
            void renderGrid(Scene& scene);
            void renderPointLight(Scene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderCamera(Scene& scene, const QMatrix4x4& modelView, const bool focus);
            void renderModel(Model& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time);
            void renderSurface(Surface& surface, Sahara::Instance& instance, const bool focus);

            void processSceneLighting(Scene& scene);
            void processControllerInstanceArmature(InstanceController& controllerInstance);
    };

}

#endif // RENDERER_H
