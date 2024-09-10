#version 450

layout(location = 0) in vec2 vertTexcoord;

layout(set = 1, binding = 0) uniform sampler2D uSampler;

layout(location = 0) out vec4 Color;

void main() {
    Color = texture(uSampler, vertTexcoord);
}
