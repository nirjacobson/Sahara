#version 100
#undef lowp
#undef mediump
#undef highp

precision highp float;

varying vec3 Color;

void main() {
    gl_FragColor = vec4(Color, 1);
}
