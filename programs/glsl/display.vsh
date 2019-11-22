#version 100

attribute vec3 position;
attribute vec3 normal;

varying vec3 vertPosition;
varying vec3 vertNormal;

struct Render {
    mat4 modelView;
    mat4 projection;
    mat4 inverseCamera;
};

uniform Render uRender;

void main() {
    vertPosition = vec3(uRender.modelView * vec4(position, 1.0));
    vertNormal = mat3(uRender.modelView) * normal;
    gl_Position = uRender.projection * uRender.inverseCamera * uRender.modelView * vec4(position, 1.0);
}
