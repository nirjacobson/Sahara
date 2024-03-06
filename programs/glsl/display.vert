#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(binding = 0) uniform VertexRender {
    layout(offset = 0) mat4 inverseCamera;
    layout(offset = 64) mat4 projection;
} uVertexRender;

layout(push_constant) uniform PushConstants {
    layout(offset = 0) mat4 modelView;
} uPushConstants;

layout(location = 0) out vec3 vertPosition;
layout(location = 1) out vec3 vertNormal;

void main() {
    vertPosition = vec3(uPushConstants.modelView * vec4(position, 1.0));
    vertNormal = mat3(uPushConstants.modelView) * normal;
    gl_Position = uVertexRender.projection * uVertexRender.inverseCamera * uPushConstants.modelView * vec4(position, 1.0);
}
