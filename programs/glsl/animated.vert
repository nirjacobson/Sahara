#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec4 joints;
layout(location = 4) in vec4 weights;

layout(location = 0) out vec3 vertPosition;
layout(location = 1) out vec3 vertNormal;
layout(location = 2) out vec2 vertTexcoord;

layout(push_constant) uniform PushConstantsVert {
    layout(offset = 0) mat4 modelView;
} uPushConstantsVert;

layout(set = 0, binding = 0) uniform Render {
    layout(offset = 0) mat4 inverseCamera;
    layout(offset = 64) mat4 projection;
} uRender;

struct Joint {
    vec4 rotation;
    vec3 translation;
};

layout(set = 4, binding = 0) uniform Armature {
    layout(offset = 0) Joint joints[110];
} uArmature;

vec3 rotate(vec4 q, vec3 p) {
    return p + 2.0 * cross(q.xyz, cross(q.xyz, p) + q.w * p);
}

void main() {
    vec3 vertPositionResult = vec3(0);
    vec3 vertNormalResult = vec3(0);

    for (int i = 0; i < 4; i++) {
        int index = int(joints[i]);
        if (index == -1) {
            break;
        }
        vec3 transformed = rotate(uArmature.joints[index].rotation, position) + uArmature.joints[index].translation;
        vertPositionResult += transformed * weights[i];
        vertNormalResult += rotate(uArmature.joints[index].rotation, normal) * weights[i];
    }

    vec4 vertPositionResult4 = vec4(vertPositionResult, 1.0);
    vertPosition = vec3(uPushConstantsVert.modelView * vertPositionResult4);
    vertNormal = mat3(uPushConstantsVert.modelView) * vertNormalResult;
    vertTexcoord = texcoord;

    gl_Position = uRender.projection * uRender.inverseCamera * uPushConstantsVert.modelView * vertPositionResult4;
}
