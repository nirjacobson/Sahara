#include "openglinstancecontroller.h"

Sahara::OpenGLInstanceController::OpenGLInstanceController(const Armature* armature, const MaterialDict& materials, const QMatrix4x4& transform, Sahara::Controller* controller)
    : InstanceController(armature, materials, transform, controller)
{

}
