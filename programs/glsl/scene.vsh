#version 300 es
#undef lowp
#undef mediump
#undef highp

precision highp float;

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec4 bones;
in vec4 weights;

out vec3 vertPosition;
out vec3 vertNormal;
out vec2 vertTexcoord;

struct Render {
    mat4 modelView;
    mat4 inverseCamera;
    mat4 projection;
    int boned;
};

uniform Render uRender;
uniform vec4 uBoneRotations[75];
uniform vec3 uBoneTranslations[75];

vec3 rotate(vec4 q, vec3 p) {
    return p + 2.0 * cross(q.xyz, cross(q.xyz, p) + q.w * p);
}

void main() {
    vec3 vertPositionResult;
    vec3 vertNormalResult;

    if (uRender.boned == 1) {
        for (int i = 0; i < 4; i++) {
            int index = int(bones[i]);
            if (index == -1) {
                break;
            }
            vec3 transformed = rotate(uBoneRotations[index], position) + uBoneTranslations[index];
            vertPositionResult += transformed * weights[i];
            vertNormalResult += rotate(uBoneRotations[index], normal) * weights[i];
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
