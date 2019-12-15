#ifndef SAHARA_CAMERA_H
#define SAHARA_CAMERA_H

#include <QMatrix4x4>
#include <QtMath>

#include "../node/nodeitem.h"
#include "asset.h"

namespace Sahara
{

    class Camera : public Asset, public NodeItem
    {
        friend class JSON;

        public:

            enum Mode {
                Perspective,
                Orthographic
            };

            Camera(const QString& id, const Mode mode, const float xfov, const float xmag, const float aspect, const float znear, const float zfar);

            Mode mode() const;
            float xfov() const;
            float xmag() const;
            float aspect() const;
            float znear() const;
            float zfar() const;

            void setMode(const Mode mode);
            void setXfov(const float xfov);
            void setXmag(const float xmag);
            void setAspect(const float aspect);
            void setZnear(const float znear);
            void setZfar(const float zfar);

            QMatrix4x4 projection();

            Volume volume() const override;

        private:
            Mode _mode;
            float _xfov;
            float _xmag;
            float _aspect;
            float _znear;
            float _zfar;
    };

}

#endif // SAHARA_CAMERA_H
