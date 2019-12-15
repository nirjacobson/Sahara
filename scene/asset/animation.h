#ifndef SAHARA_ANIMATION_H
#define SAHARA_ANIMATION_H

#include <QList>
#include <cmath>

#include "asset.h"
#include "bone.h"
#include "../../common/transform.h"

namespace Sahara
{

    class Animation : public Asset
    {
        friend class JSON;

        public:

            class Keyframe
            {
                public:
                    float time;
                    Transform transform;
            };

            Animation(const QString& id, Bone* const bone, const QList<Keyframe>& keyframes);

            void apply(const float time);

        private:
            Bone* _bone;
            QList<Keyframe> _keyframes;
            float _runningTime;
    };

}

#endif // SAHARA_ANIMATION_H
