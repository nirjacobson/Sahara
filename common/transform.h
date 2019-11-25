#ifndef SAHARA_TRANSFORM_H
#define SAHARA_TRANSFORM_H

#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

namespace Sahara {

    class Transform
    {
        public:
            Transform();
            Transform(const QQuaternion& rotation, const QVector3D& translation);
            Transform(const QMatrix4x4& transform);

            const QQuaternion& rotation() const;
            const QVector3D& translation() const;

            Transform operator*(const Transform& other) const;

            static Transform interpolate(const Transform& t1, const Transform& t2, const float t);

        private:
            QQuaternion _rotation;
            QVector3D _translation;
    };

}

#endif // SAHARA_TRANSFORM_H
