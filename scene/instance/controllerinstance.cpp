#include "controllerinstance.h"

Sahara::ControllerInstance::ControllerInstance(Sahara::Armature* armature, const Sahara::MaterialDict& materials, const QMatrix4x4& transform, Sahara::Controller* controller)
    : Instance(materials, transform)
    , _armature(armature)
    , _controller(controller)
{

}

Sahara::Controller& Sahara::ControllerInstance::controller()
{
    return *_controller;
}

const Sahara::Controller& Sahara::ControllerInstance::controller() const
{
    return *_controller;
}

const Sahara::Armature& Sahara::ControllerInstance::armature() const
{
    return *_armature;
}
