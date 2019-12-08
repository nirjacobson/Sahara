#ifndef RENDERER_H
#define RENDERER_H

#include <QStack>
#include <QMatrix4x4>

#include "../programs/sceneprogram.h"
#include "../programs/gridprogram.h"
#include "../scene/scene.h"
#include "../scene/model.h"
#include "vertexbuffer.h"
#include "grid/grid.h"
#include "../common/transform.h"

namespace Sahara {

    class Renderer
    {
        public:
            Renderer();
            ~Renderer();

            void render(Scene& scene, const float time);

            void renderGrid(const bool visible);

        private:
            SceneProgram _sceneProgram;
            GridProgram _gridProgram;

            Grid _grid;
            bool _renderGrid;

            void renderScene(Scene& scene, const float time);
            void renderGrid(Scene& scene);
            void renderModel(Model& model, const float time);
            void renderSurface(Surface& surface, Sahara::Instance& instance);

            void processSceneLighting(Scene& scene);
            void processControllerInstanceArmature(ControllerInstance& controllerInstance);
    };

}

#endif // RENDERER_H
