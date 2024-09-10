#version 300 es
#undef lowp
#undef mediump
#undef highp

precision highp float;

in vec3 Color;

out vec4 color;

void main() {
    color = vec4(Color, 1);
}
