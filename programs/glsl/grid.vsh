#version 300 es
#undef lowp
#undef mediump
#undef highp

precision highp float;

in vec3 position;
in vec3 color;

out vec3 Color;

struct Render {
    mat4 inverseCamera;
    mat4 projection;
};

uniform Render uRender;

void main() {
    Color = color;
    gl_Position = uRender.projection * uRender.inverseCamera * vec4(position, 1.0);
}
