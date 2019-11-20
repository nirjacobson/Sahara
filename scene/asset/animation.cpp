#include "animation.h"

Sahara::Animation::Animation(Sahara::Bone* const bone, const QList<Sahara::Animation::Keyframe>& keyframes)
    : _bone(bone)
    , _keyframes(keyframes)
{
    float end = _keyframes.last().time;
    float firstInterval = _keyframes.at(1).time - _keyframes.first().time;
    float lastInterval = _keyframes.last().time - _keyframes.at(_keyframes.size() - 1).time;

    _runningTime = end + (firstInterval + lastInterval) / 2.0f;
}

void Sahara::Animation::apply(const float time)
{
    float animationTime = static_cast<float>(fmod(time, _runningTime));

    int i;
    for (i = 1; i < _keyframes.size(); i++) {
        if (_keyframes.at(i).time > animationTime) {
            break;
        }
    }

    Animation::Keyframe fromFrame = _keyframes.at(i - 1);
    Animation::Keyframe toFrame = i == _keyframes.size() ? _keyframes.first() : _keyframes.at(i);

    float t = (animationTime - fromFrame.time) / (toFrame.time - fromFrame.time);
    QQuaternion rotationInterp = QQuaternion::slerp(fromFrame.rotation, toFrame.rotation, t);
    QVector3D translationInterp = fromFrame.translation + ((toFrame.translation - fromFrame.translation) * t);

    QMatrix4x4 transform( rotationInterp.toRotationMatrix() );
    transform.setColumn(3, QVector4D( translationInterp, 1.0f ));

    _bone->setTransform(transform);
}
