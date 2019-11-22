#version 100

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 bones1;
attribute vec4 bones2;
attribute vec4 weights1;
attribute vec4 weights2;

varying vec3 vertPosition;
varying vec3 vertNormal;
varying vec2 vertTexcoord;

struct Render {
    mat4 modelView;
    mat4 projection;
    mat4 inverseCamera;
    int boned;
    int focus;
};

struct Bone {
    vec4 rotation;
    vec3 translation;
};

struct Armature {
    Bone bones[74];
};

uniform Render uRender;
uniform Armature uArmature;

vec4 hamilton(vec4 q1, vec4 q2) {
    vec4 product;

    product.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
    product.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
    product.z = (q1.q * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
    product.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);

    return product;
}

vec4 conjugate(vec4 q) {
    return vec4(-q.xyz, q.w);
}

vec3 rotate(vec4 q, vec3 p) {
    vec4 temp = hamilton(q, vec4(p.xyz, 0));
    vec3 rotated = hamilton(temp, conjugate(q)).xyz;

    return rotated;
}

vec3 transform(Bone bone, vec3 vertex) {
    vec3 rotated = rotate(bone.rotation, vertex);
    vec3 rotatedAndTranslated = rotated + bone.translation;

    return rotatedAndTranslated;
}


void main() {
    vec3 vertPositionResult;

    if (uRender.boned == 1) {
        int stop = 0;
        for (int i = 0; i < 4; i++) {
            if (bones1[i] > 0.0) {
                vertPositionResult += transform(uArmature.bones[int(bones1[i])], position) * weights1[i];
            } else {
                stop = 1;
                break;
            }
        }
        if (stop == 0) {
            for (int i = 0; i < 4; i++) {
                if (bones2[i] > 0.0) {
                    vertPositionResult += transform(uArmature.bones[int(bones2[i])], position) * weights2[i];
                }
            }
        }
    } else {
        vertPositionResult = position;
    }

    if (uRender.focus == 1) {
        vertPositionResult *= 1.01;
    }

    vec4 vertPositionResult4 = vec4(vertPositionResult, 1.0);
    vertPosition = vec3(uRender.modelView * vertPositionResult4);
    vertNormal = mat3(uRender.modelView) * normal;
    vertTexcoord = texcoord;

    gl_Position = uRender.projection * uRender.inverseCamera * uRender.modelView * vertPositionResult4;
}
