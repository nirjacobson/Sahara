#ifndef SAHARA_CAMERACONTROL_H
#define SAHARA_CAMERACONTROL_H

#include <QVector2D>
#include <QVector3D>

#include "../../Sahara_global.h"
#include "../../scene/node/node.h"

namespace Sahara {

class SAHARA_EXPORT CameraControl
    {
        private:
            struct Acceleration {
                    bool forward;
                    bool backward;
                    bool left;
                    bool right;
            };
            typedef struct Acceleration Acceleration;

        public:
            CameraControl();

            const QVector2D& rotationalVelocity() const;
            void setRotationalVelocity(const QVector2D& velocity);

            void accelerateForward(const bool on);
            void accelerateBackward(const bool on);
            void accelerateLeft(const bool on);
            void accelerateRight(const bool on);
            void reset();

            QMatrix4x4 update(Node& cameraNode);

        private:
            QVector2D _rotationalVelocity;
            QVector3D _velocity;
            Acceleration _acceleration;
    };

}

#endif // SAHARA_CAMERACONTROL_H
