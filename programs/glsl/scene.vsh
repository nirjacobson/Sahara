#version 100

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
uniform vec4 uBoneRotations[67];
uniform vec3 uBoneTranslations[67];

vec4 hamilton(vec4 q1, vec4 q2) {
    vec4 product;

    product.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
    product.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
    product.z = (q1.q * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
    product.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);

    return product;
}

vec4 inverse(vec4 q) {
    return vec4(-q.xyz, q.w);
}

vec3 rotate(vec4 q, vec3 p) {
    vec4 temp = hamilton(q, vec4(p.xyz, 0));
    vec3 rotated = hamilton(temp, inverse(q)).xyz;

    return rotated;
}

void main() {
    vec3 vertPositionResult;

    if (uRender.boned == 1) {
        float sum = 0.0;
        for (int i = 0; i < 4; i++) {
            sum += weights[i];
        }
        for (int i = 0; i < 4; i++) {
            int index = int(bones[i]);
            if (index == -1) {
                break;
            }
            vec3 rotatedAndTranslated = rotate(uBoneRotations[index], position) + uBoneTranslations[index];
            vertPositionResult += rotatedAndTranslated * (weights[i]/sum);
        }
    } else {
        vertPositionResult = position;
    }

    if (uFocus == 1) {
        vertPositionResult *= 1.01;
    }

    vec4 vertPositionResult4 = vec4(vertPositionResult, 1.0);
    vertPosition = vec3(uRender.modelView * vertPositionResult4);
    vertNormal = mat3(uRender.modelView) * normal;
    vertTexcoord = texcoord;

    gl_Position = uRender.projection * uRender.inverseCamera * uRender.modelView * vertPositionResult4;
}
