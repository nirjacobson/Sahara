#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

layout(location = 0) out vec2 vertTexcoord;

layout(push_constant) uniform PushConstantsVert {
    layout(offset = 0) vec2 location;
} uPushConstantsVert;

layout(set = 0, binding = 0) uniform Render {
    layout(offset = 0) mat4 projection;
} uRender;

void main() {
    mat4 modelView = mat4(1.0);
    modelView[3] = vec4(uPushConstantsVert.location, 0, 1);

    vertTexcoord = texcoord;
    gl_Position = uRender.projection * modelView * vec4(position, 0, 1);
}
