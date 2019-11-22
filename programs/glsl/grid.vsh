#version 100

attribute vec3 position;

struct Render {
    mat4 inverseCamera;
    mat4 projection;
};

uniform Render uRender;

void main() {
    gl_Position = uRender.projection * uRender.inverseCamera * vec4(position, 1.0);
}
