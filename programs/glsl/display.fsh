#version 300 es
#undef lowp
#undef mediump
#undef highp

precision highp float;

in vec3 vertPosition;
in vec3 vertNormal;

out vec4 color;

struct FragmentRender {
    vec3 cameraPosition;
    int focus;
};

uniform FragmentRender uFragmentRender;

void main() {
    vec3 diffuse;
    if (uFragmentRender.focus == 0) {
        diffuse = vec3(0.5, 0.5, 0.5);
        vec3 lightPosition = uFragmentRender.cameraPosition;
        vec3 toLight = normalize(lightPosition - vertPosition);
        color = vec4(diffuse * max(dot(vertNormal, toLight), 0.0), 1.0);
    } else {
        color = vec4(1, 0.5765, 0, 1);
    }
}
