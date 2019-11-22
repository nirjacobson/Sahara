#version 100

varying vec3 vertPosition;
varying vec3 vertNormal;

uniform vec3 uCameraPosition;
uniform int uFocus;

void main() {
    vec3 diffuse;
    if (uFocus == 0) {
        diffuse = vec3(0.5, 0.5, 0.5);
        vec3 lightPosition = uCameraPosition;
        vec3 toLight = normalize(lightPosition - vertPosition);
        gl_FragColor = vec4(diffuse * max(dot(vertNormal, toLight), 0.0), 1.0);
    } else {
        gl_FragColor = vec4(1, 0.5765, 0, 1);
    }
}
