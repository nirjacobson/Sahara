#version 100

precision highp float;
precision highp int;

varying vec3 vertPosition;
varying vec3 vertNormal;
varying vec2 vertTexcoord;

uniform vec3 uCameraPosition;
uniform int uFocus;

struct PointLight {
    vec3 position;
    vec3 color;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

struct Lighting {
    PointLight pointLights[6];
    int pointLightCount;
};

struct Material {
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    int textured;
};

uniform Lighting uLighting;
uniform Material uMaterial;
uniform sampler2D uSampler;

void main() {
    vec3 normal = normalize(vertNormal);
    vec4 outputColor;

    if (uFocus == 0) {
        outputColor = vec4(0, 0, 0, 0);

        if (uLighting.pointLightCount == 0) {
           outputColor.a = 1.0;
        }

        for (int i = 0; i < 6; i++) {
            if (i == uLighting.pointLightCount) {
                break;
            }

            vec3 toLight = uLighting.pointLights[i].position - vertPosition;
            vec3 toLightN = normalize(toLight);
            vec3 toCamera = normalize(uCameraPosition - vertPosition);
            vec3 reflection = reflect(-toLightN, normal);

            vec4 d;
            if (uMaterial.textured > 0) {
                d = texture2D(uSampler, vertTexcoord);
            } else {
                d = uMaterial.diffuse;
            }

            vec3 emission = uMaterial.emission.rgb;
            vec3 ambient = uMaterial.ambient.rgb;
            float diff = max(dot(normal, toLightN), 0.0);
            vec3 diffuse = diff * uLighting.pointLights[i].color;
            float spec = pow(max(dot(toCamera, reflection), 0.0), uMaterial.shininess);
            vec3 specular = spec * uLighting.pointLights[i].color * uMaterial.specular.rgb;

            float distancetoLight = length(toLight);
            float attenuation = 1.0 / (uLighting.pointLights[i].constantAttenuation + (uLighting.pointLights[i].linearAttenuation + (uLighting.pointLights[i].quadraticAttenuation * distancetoLight)) * distancetoLight);

            vec3 color = (emission + ambient + attenuation * (diffuse + specular)) * d.rgb;

            vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);

            outputColor += vec4(pow(color, gamma), d.a);
        }
    } else {
        outputColor = vec4(1, 0.5765, 0, 0.5);
    }

    gl_FragColor = outputColor;
}
