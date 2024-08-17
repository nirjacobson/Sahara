#ifndef SAHARA_OPENGL_INSTANCECONTROLLER_H
#define SAHARA_OPENGL_INSTANCECONTROLLER_H

#include "instancecontroller.h"
namespace Sahara
{
    class OpenGLInstanceController : public InstanceController
    {
        friend class JSON;

        public:
            OpenGLInstanceController(const Armature* armature, const MaterialDict& materials, const QMatrix4x4& transform, Controller* controller);
    };

}

#endif // SAHARA_OPENGL_INSTANCECONTROLLER_H
