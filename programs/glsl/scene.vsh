#version 100

precision highp float;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 bones;
attribute vec4 weights;

varying vec3 vertPosition;
varying vec3 vertNormal;
varying vec2 vertTexcoord;

uniform int uFocus;

struct Render {
    mat4 modelView;
    mat4 inverseCamera;
    mat4 projection;
    int boned;
};

uniform Render uRender;
uniform vec4 uBoneRotations[66];
uniform vec3 uBoneTranslations[66];

vec3 rotate(vec4 q, vec3 p) {
    return p + 2.0 * cross(q.xyz, cross(q.xyz, p) + q.w * p);
}

void main() {
    vec3 vertPositionResult;
    vec3 vertNormalResult;

    if (uRender.boned == 1) {
        float sum = 0.0;
        for (int i = 0; i < 3; i++) {
            int index = int(bones[i]);
            if (index == -1) {
                break;
            }
            sum += weights[i];
        }
        for (int i = 0; i < 3; i++) {
            int index = int(bones[i]);
            if (index == -1) {
                break;
            }
            vec3 transformed = rotate(uBoneRotations[index], position) + uBoneTranslations[index];
            vertPositionResult += transformed * weights[i] / sum;
            vertNormalResult += rotate(uBoneRotations[index], normal) * weights[i] / sum;
        }
    } else {
        vertPositionResult = position;
        vertNormalResult = normal;
    }

    if (uFocus == 1) {
        vertPositionResult *= 1.01;
    }

    vec4 vertPositionResult4 = vec4(vertPositionResult, 1.0);
    vertPosition = vec3(uRender.modelView * vertPositionResult4);
    vertNormal = mat3(uRender.modelView) * vertNormalResult;
    vertTexcoord = texcoord;

    gl_Position = uRender.projection * uRender.inverseCamera * uRender.modelView * vertPositionResult4;
}
