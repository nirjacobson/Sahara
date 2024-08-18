#ifndef MODEL_H
#define MODEL_H

#include "Sahara_global.h"

#include <QFileInfo>
#include <QDir>

#include "node/nodeitem.h"

#include "scene/asset/controller.h"
#include "scene/asset/armature.h"
#include "scene/asset/animation.h"
#include "scene/asset/animationclip.h"
#include "scene/instance/instance.h"
#include "scene/node/nodeitem.h"
#include "scene/asset/image.h"
#include "common/volume.h"

namespace Sahara {

    typedef QMap<QString, Image*> ImageDict;
    typedef QMap<QString, Material*> MaterialDict;
    typedef QMap<QString, Mesh*> MeshDict;
    typedef QMap<QString, Controller*> ControllerDict;
    typedef QMap<QString, Animation*> AnimationDict;
    typedef QMap<QString, AnimationClip*> AnimationClipDict;

    class SAHARA_EXPORT Model : public NodeItem
    {    friend class JSON;

    protected:
        Model();

    public:
        ~Model();

        Volume volume() const override;

        const ImageDict& images() const;
        const MaterialDict& materials() const;
        const MeshDict& meshes() const;
        const ControllerDict& controllers() const;
        const QList<Instance*> instances() const ;
        const AnimationDict& animations() const;
        const AnimationClipDict& animationClips() const;
        const Armature& armature() const;

        QStringList animationClipNames() const;
        void setAnimationClip(const QString& name);
        QString animationClip() const;

        int triangles() const;

        void animate(const float time);

    protected:
        Volume _volume;
        ImageDict _images;
        MaterialDict _materials;
        MeshDict _meshes;
        ControllerDict _controllers;
        AnimationDict _animations;
        AnimationClipDict _animationClips;

        QList<Instance*> _instances;
        Armature* _armature;
        AnimationClip* _animationClip;

    };
}

#endif // MODEL_H
