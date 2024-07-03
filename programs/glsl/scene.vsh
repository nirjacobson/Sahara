#version 100
#undef lowp
#undef mediump
#undef highp

precision highp float;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 joints;
attribute vec4 weights;

varying vec3 vertPosition;
varying vec3 vertNormal;
varying vec2 vertTexcoord;

struct Render {
    mat4 modelView;
    mat4 inverseCamera;
    mat4 projection;
};

struct Joint {
    vec4 rotation;
    vec3 translation;
};

struct Armature {
    Joint joints[110];
    int present;
};

uniform Render uRender;
uniform Armature uArmature;

vec3 rotate(vec4 q, vec3 p) {
    return p + 2.0 * cross(q.xyz, cross(q.xyz, p) + q.w * p);
}

void main() {
    vec3 vertPositionResult = vec3(0);
    vec3 vertNormalResult = vec3(0);

    if (uArmature.present == 1) {
        for (int i = 0; i < 4; i++) {
            int index = int(joints[i]);
            if (index == -1) {
                break;
            }
            vec3 transformed = rotate(uArmature.joints[index].rotation, position) + uArmature.joints[index].translation;
            vertPositionResult += transformed * weights[i];
            vertNormalResult += rotate(uArmature.joints[index].rotation, normal) * weights[i];
        }
    } else {
        vertPositionResult = position;
        vertNormalResult = normal;
    }

    vec4 vertPositionResult4 = vec4(vertPositionResult, 1.0);
    vertPosition = vec3(uRender.modelView * vertPositionResult4);
    vertNormal = mat3(uRender.modelView) * vertNormalResult;
    vertTexcoord = texcoord;

    gl_Position = uRender.projection * uRender.inverseCamera * uRender.modelView * vertPositionResult4;
}
