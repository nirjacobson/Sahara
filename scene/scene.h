#ifndef SAHARA_SCENE_H
#define SAHARA_SCENE_H

#include "Sahara_global.h"

#include "asset/camera.h"
#include "asset/light/light.h"
#include "asset/light/ambientlight.h"
#include "model.h"
#include "node/node.h"

namespace Sahara {
    class SAHARA_EXPORT Scene
    {
        friend class JSON;

    public:
        Scene();
        ~Scene();

        const Node& root() const;
        Node& root();

        const QMap<QString, Camera*>& cameras() const;
        const QMap<QString, Light*>& lights() const;
        const QMap<QString, Model*>& models() const;

        void addCamera(const QString& id, Camera* const camera);
        void addLight(const QString& id, Light* const light);
        void addModel(const QString& id, Model* const model);

        void setCameraNode(Sahara::Node* node);
        Node& cameraNode();
        const Node& cameraNode() const;
        Camera& camera();
        const Camera& camera() const;
        Node* focusNode();

        const AmbientLight& ambientLight() const;
        AmbientLight& ambientLight();

    protected:
        AmbientLight _ambientLight;

        Node* _root;

        Node* _cameraNode;

        QMap<QString, Camera*> _cameras;
        QMap<QString, Light*> _lights;
        QMap<QString, Model*> _models;
    };
}

#endif // SAHARA_SCENE_H
