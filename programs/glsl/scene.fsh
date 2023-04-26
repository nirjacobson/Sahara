#version 300 es
#undef lowp
#undef mediump
#undef highp

precision highp float;

in vec3 vertPosition;
in vec3 vertNormal;
in vec2 vertTexcoord;

out vec4 Color;

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

    if (uFocus == 0) {
        vec4 d;
        if (uMaterial.textured > 0) {
            d = texture2D(uSampler, vertTexcoord);
        } else {
            d = uMaterial.diffuse;
        }

        vec3 outputColor = uMaterial.emission.rgb;

        for (int i = 0; i < 6; i++) {
            if (i == uLighting.pointLightCount) {
                break;
            }

            vec3 toLight = uLighting.pointLights[i].position - vertPosition;
            vec3 toLightN = normalize(toLight);
            vec3 toCamera = normalize(uCameraPosition - vertPosition);
            vec3 reflection = reflect(-toLightN, normal);

            vec3 ambient = uMaterial.ambient.rgb;
            float diff = max(dot(normal, toLightN), 0.0);
            vec3 diffuse = diff * d.rgb;
            float spec = pow(max(dot(toCamera, reflection), 0.0), uMaterial.shininess);
            vec3 specular = spec * uMaterial.specular.rgb;

            float distancetoLight = length(toLight);
            float attenuation = 1.0 / (uLighting.pointLights[i].constantAttenuation + (uLighting.pointLights[i].linearAttenuation + (uLighting.pointLights[i].quadraticAttenuation * distancetoLight)) * distancetoLight);

            outputColor += (ambient + attenuation * (diffuse + specular)) * uLighting.pointLights[i].color;;
        }

        vec3 gamma = vec3(1.0/2.2);

        Color = vec4(pow(outputColor, gamma), d.a);
    } else {
        Color = vec4(1, 0.5765, 0, 0.5);
    }
}
