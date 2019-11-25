#ifndef RENDERER_H
#define RENDERER_H

#include <QStack>
#include <QMatrix4x4>

#include "../programs/sceneprogram.h"
#include "../scene/scene.h"
#include "../scene/model.h"
#include "vertexbuffer.h"
#include "../common/transform.h"

namespace Sahara {

    class Renderer
    {
        public:
            Renderer();
            ~Renderer();

            void render(Scene& scene, const float time);

        private:
            SceneProgram _sceneProgram;

            void renderScene(Scene& scene, const float time);
            void renderModel(Model& model, const float time);
            void renderSurface(Surface& surface, Sahara::Instance& instance);

            void processSceneLighting(Scene& scene);
            void processControllerInstanceArmature(ControllerInstance& controllerInstance);
    };

}

#endif // RENDERER_H
