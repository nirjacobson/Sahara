#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

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


void main() {
    vec4 vertPositionResult4 = vec4(position, 1.0);
    vertPosition = vec3(uPushConstantsVert.modelView * vertPositionResult4);
    vertNormal = mat3(uPushConstantsVert.modelView) * normal;
    vertTexcoord = texcoord;

    gl_Position = uRender.projection * uRender.inverseCamera * uPushConstantsVert.modelView * vertPositionResult4;
}
