#include "cameracontrol.h"

Sahara::CameraControl::CameraControl()
{
    reset();
}

const QVector2D&Sahara::CameraControl::rotationalVelocity() const
{
    return _rotationalVelocity;
}

void Sahara::CameraControl::setRotationalVelocity(const QVector2D& velocity)
{
    _rotationalVelocity = velocity;
}

void Sahara::CameraControl::accelerateForward(const bool on)
{
    _acceleration.forward = on;
}

void Sahara::CameraControl::accelerateBackward(const bool on)
{
    _acceleration.backward = on;
}

void Sahara::CameraControl::accelerateLeft(const bool on)
{
    _acceleration.left = on;
}

void Sahara::CameraControl::accelerateRight(const bool on)
{
    _acceleration.right = on;
}

void Sahara::CameraControl::reset()
{
    _rotationalVelocity = { 0, 0 };
    _velocity = { 0, 0, 0 };
    _acceleration = { false, false, false, false, };
}

void Sahara::CameraControl::update(Sahara::Node& cameraNode)
{
    QVector3D right = { 1, 0, 0 };
    QVector3D up = { 0, 1, 0 };
    QVector3D forward = { 0, 0, -1 };

    QMatrix3x3 nodeRotation = cameraNode.transform().toGenericMatrix<3, 3>();
    QMatrix4x4 nodeTranslation;
    nodeTranslation.setColumn(3, cameraNode.transform().column(3));

    right = QMatrix4x4(nodeRotation).map(right);
    forward = QMatrix4x4(nodeRotation).map(forward);

    QMatrix4x4 rotation;
    rotation.rotate(_rotationalVelocity.y(), right);
    rotation.rotate(-_rotationalVelocity.x(), up);

    QMatrix4x4 translation;
    translation.translate(_velocity);

    cameraNode.setTransform(translation * nodeTranslation * rotation * QMatrix4x4(nodeRotation));

    QVector3D acceleration;
    if (_acceleration.forward)
        acceleration += forward;
    if (_acceleration.backward)
        acceleration -= forward;
    if (_acceleration.left)
        acceleration -= right;
    if (_acceleration.right)
        acceleration += right;

    acceleration *= 0.02f;

    if (_velocity.length() > 0) {
        _velocity *= 0.95f;
    }

    _velocity = _velocity + acceleration;
}
