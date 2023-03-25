#include "camera.h"


Sahara::Camera::Camera(const QString& id, const Sahara::Camera::Mode mode, const float xfov, const float xmag, const float aspect, const float znear, const float zfar)
    : Asset(id)
    , _mode(mode)
    , _xfov(xfov)
    , _xmag(xmag)
    , _aspect(aspect)
    , _znear(znear)
    , _zfar(zfar)
{

}

Sahara::Camera::Mode Sahara::Camera::mode() const
{
    return _mode;
}

float Sahara::Camera::xfov() const
{
    return _xfov;
}

float Sahara::Camera::xmag() const
{
    return _xmag;
}

float Sahara::Camera::aspect() const
{
    return _aspect;
}

float Sahara::Camera::znear() const
{
    return _znear;
}

float Sahara::Camera::zfar() const
{
    return _zfar;
}

void Sahara::Camera::setMode(const Sahara::Camera::Mode mode)
{
    _mode = mode;
}

void Sahara::Camera::setXfov(const float xfov)
{
    _xfov = xfov;
}

void Sahara::Camera::setXmag(const float xmag)
{
    _xmag = xmag;
}

void Sahara::Camera::setAspect(const float aspect)
{
    _aspect = aspect;
}

void Sahara::Camera::setZnear(const float znear)
{
    _znear = znear;
}

void Sahara::Camera::setZfar(const float zfar)
{
    _zfar = zfar;
}


QMatrix4x4 Sahara::Camera::projection()
{
    QMatrix4x4 projection;
    if (_mode == Mode::Perspective) {
        float xfovRad = qDegreesToRadians(_xfov);
        double yfov = 2 * qAtan(qTan(static_cast<double>(xfovRad)/2.0) / static_cast<double>(_aspect));
        float yfovDeg = static_cast<float>(qRadiansToDegrees(yfov));

        projection.perspective(yfovDeg, _aspect, _znear, _zfar);
    } else {
        float right = _xmag / 2.0f;
        float left = -right;
        float top = right / _aspect;
        float bottom = -top;

        projection.ortho(left, right, bottom, top, _znear, _zfar);
    }

    return projection;
}

Sahara::Volume Sahara::Camera::volume() const
{
    return {{-1, -1, -2}, {1, 1, 0}};
}
