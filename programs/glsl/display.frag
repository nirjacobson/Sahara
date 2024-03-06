#version 450

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertNormal;

layout(push_constant) uniform FragmentRender {
    layout(offset = 128) vec3 cameraPosition;
    layout(offset = 144) int focus;
} uFragmentRender;

layout(location = 0) out vec4 color;

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
