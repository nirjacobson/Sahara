#version 100
#undef lowp
#undef mediump
#undef highp

precision highp float;

attribute vec3 position;
attribute vec3 color;

varying vec3 Color;

struct Render {
    mat4 inverseCamera;
    mat4 projection;
};

uniform Render uRender;

void main() {
    Color = color;
    gl_Position = uRender.projection * uRender.inverseCamera * vec4(position, 1.0);
}
