#include "instancecontroller.h"
#include "render/renderer.h"

Sahara::InstanceController::InstanceController(Renderer* renderer, const Armature* armature, const Sahara::MaterialDict& materials, const QMatrix4x4& transform, Sahara::Controller* controller)
    : Instance(materials, transform)
    , _renderer(renderer)
    , _armature(armature)
    , _controller(controller)
{
    _uniformBuffers = renderer->createArmatureUniformBuffers();
}

Sahara::InstanceController::~InstanceController()
{
    _renderer->destroyUniformBuffers(_uniformBuffers);
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

const QList<VkDescriptorSet> &Sahara::InstanceController::descriptorSets() const
{
    return _uniformBuffers.bufferDescriptorSets;
}

void Sahara::InstanceController::updateUniform() const
{
    QList<Transform> jointTransforms;
    for (int i = 0; i < _controller->joints().size(); i++) {
        QString jointName = _controller->joints().at(i);
        const Joint* joint = _armature->getJointByName(jointName);
        QMatrix4x4 inverseBindMatrix = _controller->inverseBindMatrices().at(i);
        QMatrix4x4 matrix = inverseBindMatrix * _controller->bindShapeMatrix();

        Transform transform = Transform(matrix);

        while (joint) {
            transform = joint->transform() * transform;
            joint = &joint->parent();
        }

        jointTransforms.push_back(transform);
    }

    ScenePipeline::Armature armature;
    for (int i = 0; i < jointTransforms.size(); i++) {
        QVector4D vec4 = jointTransforms[i].rotation().toVector4D();
        QVector3D vec3 = jointTransforms[i].translation();
        armature.joints[i] = {
            .rotation = {
                vec4.x(),
                vec4.y(),
                vec4.z(),
                vec4.w()
            },
            .translation = {
                vec3.x(),
                vec3.y(),
                vec3.z()
            }
        };
    }

    for (int i = 0; i < _uniformBuffers.buffers.size(); i++) {
        memcpy(_uniformBuffers.buffersMapped[i], &armature, sizeof(ScenePipeline::Armature));
    }
}
