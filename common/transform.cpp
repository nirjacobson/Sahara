#include "transform.h"

Sahara::Transform::Transform()
{

}

Sahara::Transform::Transform(const QQuaternion& rotation, const QVector3D& translation)
    : _rotation(rotation)
    , _translation(translation)
{

}

Sahara::Transform::Transform(const QMatrix4x4& transform)
{
    _rotation = QQuaternion::fromRotationMatrix(transform.toGenericMatrix<3, 3>());
    _translation = QVector3D(transform.column(3));
}

const QQuaternion& Sahara::Transform::rotation() const
{
    return _rotation;
}

const QVector3D& Sahara::Transform::translation() const
{
    return _translation;
}

Sahara::Transform Sahara::Transform::operator*(const Sahara::Transform& other) const
{
    QQuaternion rotation = _rotation * other._rotation;
    QVector3D translation = _rotation * other._translation + _translation;

    return Transform(rotation, translation);
}

Sahara::Transform Sahara::Transform::interpolate(const Sahara::Transform& t1, const Sahara::Transform& t2, const float t)
{
    QQuaternion rotation = QQuaternion::slerp(t1.rotation(), t2.rotation(), t);
    QVector3D translation = t1.translation() + ((t2.translation() - t1.translation()) * t);

    return Transform(rotation, translation);
}
