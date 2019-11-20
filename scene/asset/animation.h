#ifndef SAHARA_ANIMATION_H
#define SAHARA_ANIMATION_H

#include <QList>
#include <QMatrix4x3>
#include <cmath>

#include "asset.h"
#include "bone.h"

namespace Sahara
{

    class Animation : public Asset
    {
        public:

            class Keyframe
            {
                public:
                    float time;
                    QQuaternion rotation;
                    QVector3D translation;
            };

            Animation(Bone* const bone, const QList<Keyframe>& keyframes);

            void apply(const float time);

        private:
            Bone* _bone;
            QList<Keyframe> _keyframes;
            float _runningTime;
    };

}

#endif // SAHARA_ANIMATION_H
