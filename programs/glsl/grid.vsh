#version 100

attribute vec3 position;

struct Render {
    mat4 projection;
    mat4 inverseCamera;
};

uniform Render uRender;

void main() {
    gl_Position = uRender.projection * uRender.inverseCamera * vec4(position, 1.0);
}
