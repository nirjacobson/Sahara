#version 120
#undef lowp
#undef mediump
#undef highp

varying vec3 Color;

void main() {
    gl_FragColor = vec4(Color, 1);
}
