#include "instancecontroller.h"


Sahara::InstanceController::InstanceController(const Armature* armature, const MaterialDict& materials, const QMatrix4x4& transform, Controller* controller)
    : Instance(materials, transform)
    , _armature(armature)
    , _controller(controller)
{

}

Sahara::Volume Sahara::InstanceController::volume() const
{
    QVector3D lowerVertex(1000, 1000, 1000);
    QVector3D upperVertex(-1000, -1000, -1000);

    for (int s = 0; s < _controller->mesh().count(); s++) {
        if (_controller->mesh().surface(s).volume().lowerVertex().x() < lowerVertex.x()) {
            lowerVertex.setX(_controller->mesh().surface(s).volume().lowerVertex().x());
        }
        if (_controller->mesh().surface(s).volume().lowerVertex().y() < lowerVertex.y()) {
            lowerVertex.setY(_controller->mesh().surface(s).volume().lowerVertex().y());
        }
        if (_controller->mesh().surface(s).volume().lowerVertex().z() < lowerVertex.z()) {
            lowerVertex.setZ(_controller->mesh().surface(s).volume().lowerVertex().z());
        }


        if (_controller->mesh().surface(s).volume().upperVertex().x() > upperVertex.x()) {
            upperVertex.setX(_controller->mesh().surface(s).volume().upperVertex().x());
        }
        if (_controller->mesh().surface(s).volume().upperVertex().y() > upperVertex.y()) {
            upperVertex.setY(_controller->mesh().surface(s).volume().upperVertex().y());
        }
        if (_controller->mesh().surface(s).volume().upperVertex().z() > upperVertex.z()) {
            upperVertex.setZ(_controller->mesh().surface(s).volume().upperVertex().z());
        }
    }

    return Volume(lowerVertex, upperVertex);
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
