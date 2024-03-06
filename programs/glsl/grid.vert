#version 450

layout(location = 0) in vec3 position;
layout(location = 1)in vec3 color;

layout(binding = 0) uniform Render {
    layout(offset = 0) mat4 inverseCamera;
    layout(offset = 64) mat4 projection;
} uRender;

layout(location = 0) out vec3 Color;

void main() {
    Color = color;
    gl_Position = uRender.projection * uRender.inverseCamera * vec4(position, 1.0);
}
