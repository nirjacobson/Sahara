#version 100

attribute vec3 position;
attribute vec3 normal;

varying vec3 vertPosition;
varying vec3 vertNormal;

struct VertexRender {
    mat4 modelView;
    mat4 inverseCamera;
    mat4 projection;
};

uniform VertexRender uVertexRender;

void main() {
    vertPosition = vec3(uVertexRender.modelView * vec4(position, 1.0));
    vertNormal = mat3(uVertexRender.modelView) * normal;
    gl_Position = uVertexRender.projection * uVertexRender.inverseCamera * uVertexRender.modelView * vec4(position, 1.0);
}
