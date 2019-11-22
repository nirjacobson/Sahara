#version 100

varying vec3 vertPosition;
varying vec3 vertNormal;

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
        gl_FragColor = vec4(diffuse * max(dot(vertNormal, toLight), 0.0), 1.0);
    } else {
        gl_FragColor = vec4(1, 0.5765, 0, 1);
    }
}
