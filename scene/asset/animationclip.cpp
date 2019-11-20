#include "animationclip.h"


Sahara::AnimationClip::AnimationClip(const QString& name, const QList<Sahara::Animation*>& animations)
    : _name(name)
    , _animations(animations)
{

}

const QString& Sahara::AnimationClip::name() const
{
    return _name;
}

void Sahara::AnimationClip::apply(const float time)
{
    for (Animation* animation : _animations) {
        animation->apply(time);
    }
}
