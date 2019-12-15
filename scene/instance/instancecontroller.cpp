#include "instancecontroller.h"

Sahara::InstanceController::InstanceController(const Armature* armature, const Sahara::MaterialDict& materials, const QMatrix4x4& transform, Sahara::Controller* controller)
    : Instance(materials, transform)
    , _armature(armature)
    , _controller(controller)
{

}

Sahara::Controller& Sahara::InstanceController::controller()
{
    return *_controller;
}

const Sahara::Controller& Sahara::InstanceController::controller() const
{
    return *_controller;
}

const Sahara::Armature& Sahara::InstanceController::armature() const
{
    return *_armature;
}
