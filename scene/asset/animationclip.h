#ifndef SAHARA_ANIMATIONCLIP_H
#define SAHARA_ANIMATIONCLIP_H

#include <QString>
#include <QList>

#include "animation.h"
#include "asset.h"

namespace Sahara
{

    class AnimationClip : public Asset
    {
        public:
            AnimationClip(const QString& name, const QList<Animation*>& animations);

            const QString& name() const;

            void apply(const float time);

        private:
            QString _name;
            QList<Animation*> _animations;
    };

}

#endif // SAHARA_ANIMATIONCLIP_H
