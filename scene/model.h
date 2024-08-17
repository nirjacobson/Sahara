#ifndef MODEL_H
#define MODEL_H

#include <QFileInfo>
#include <QDir>

#include "node/nodeitem.h"

#include "asset/controller.h"
#include "asset/armature.h"
#include "asset/animation.h"
#include "asset/animationclip.h"
#include "instance/instance.h"
#include "node/nodeitem.h"
#include "asset/openglimage.h"
#include "collada/collada.h"
#include "common/volume.h"

namespace Sahara {

    typedef QMap<QString, Image*> ImageDict;
    typedef QMap<QString, Material*> MaterialDict;
    typedef QMap<QString, Mesh*> MeshDict;
    typedef QMap<QString, Controller*> ControllerDict;
    typedef QMap<QString, Animation*> AnimationDict;
    typedef QMap<QString, AnimationClip*> AnimationClipDict;

    class Model : public NodeItem
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
