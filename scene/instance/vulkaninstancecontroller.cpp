#include "vulkaninstancecontroller.h"
#include "render/vulkanrenderer.h"

Sahara::VulkanInstanceController::VulkanInstanceController(VulkanRenderer* renderer, const Armature* armature, const Sahara::MaterialDict& materials, const QMatrix4x4& transform, Sahara::Controller* controller)
    : InstanceController(armature, materials, transform, controller)
    , _renderer(renderer)
{
    _uniformBuffers = renderer->createArmatureUniformBuffers();
}

Sahara::VulkanInstanceController::~VulkanInstanceController()
{
    _renderer->destroyArmatureUniformBuffers(_uniformBuffers);
}

const QList<VkDescriptorSet> &Sahara::VulkanInstanceController::descriptorSets() const
{
    return _uniformBuffers.bufferDescriptorSets;
}

void Sahara::VulkanInstanceController::updateUniform(const uint32_t currentFrame)
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

    AnimatedPipeline::Armature armature;
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

    memcpy(_uniformBuffers.buffersMapped[currentFrame], &armature, sizeof(AnimatedPipeline::Armature));
}

void Sahara::VulkanInstanceController::recreateUniform()
{
    _uniformBuffers = _renderer->createArmatureUniformBuffers();
}
