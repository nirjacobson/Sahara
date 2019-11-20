#include "camera.h"


Sahara::Camera::Camera(const Sahara::Camera::Mode mode, const float xfov, const float xmag, const float aspect, const float znear, const float zfar)
    : _mode(mode)
    , _xfov(xfov)
    , _xmag(xmag)
    , _aspect(aspect)
    , _znear(znear)
    , _zfar(zfar)
{

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
    return {{-1, -1, -2}, {1, 2.066667f, 1}};
}
