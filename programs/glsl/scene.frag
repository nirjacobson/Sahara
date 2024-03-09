#version 450

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertTexcoord;

layout(location = 0) out vec4 Color;

layout(push_constant) uniform PushConstantsFrag {
    layout(offset = 64) vec3 cameraPosition;
    layout(offset = 80) int focus;
} uPushConstantsFrag;

struct AmbientLight {
    vec3 color;
    float strength;
};

struct PointLight {
    vec3 position;
    vec3 color;
    vec3 attenuation;
};

layout(set = 1, binding = 0) uniform Lighting {
    layout(offset = 0) AmbientLight ambientLight;
    layout(offset = 16) PointLight pointLights[6];
    layout(offset = 304) int pointLightCount;
} uLighting;

layout(set = 2, binding = 0) uniform Material {
    layout(offset = 0) vec4 emission;
    layout(offset = 16) vec4 ambient;
    layout(offset = 32) vec4 diffuse;
    layout(offset = 48) vec4 specular;
    layout(offset = 64) float shininess;
    layout(offset = 80) int textured;
} uMaterial;

layout(set = 3, binding = 0) uniform sampler2D uSampler;

void main() {
    vec3 normal = normalize(vertNormal);

    if (uPushConstantsFrag.focus == 0) {
        vec4 d;
        if (uMaterial.textured > 0) {
            d = texture(uSampler, vertTexcoord);
        } else {
            d = uMaterial.diffuse;
        }

        vec3 outputColor = uLighting.ambientLight.strength * uLighting.ambientLight.color * uMaterial.ambient.rgb;
        outputColor += uMaterial.emission.rgb;

        for (int i = 0; i < 6; i++) {
            if (i == uLighting.pointLightCount) {
                break;
            }

            vec3 toLight = uLighting.pointLights[i].position - vertPosition;
            vec3 toLightN = normalize(toLight);
            vec3 toCamera = normalize(uPushConstantsFrag.cameraPosition - vertPosition);
            vec3 reflection = reflect(-toLightN, normal);

            float diff = max(dot(normal, toLightN), 0.0);
            vec3 diffuse = diff * d.rgb;
            float spec = max(dot(toCamera, reflection), 0.0);
            if (uMaterial.shininess != 0.0) {
                 spec = pow(spec, uMaterial.shininess);
            }
            vec3 specular = spec * uMaterial.specular.rgb;

            float dist = length(toLight);
            float l = uLighting.pointLights[i].attenuation[1];
            float q = uLighting.pointLights[i].attenuation[2] * dist;
            float lqd = (l + q) * dist;
            float c = uLighting.pointLights[i].attenuation[0];

            float attenuation = 1.0 / (c + lqd);

            outputColor += attenuation * ((diffuse + specular) * uLighting.pointLights[i].color);
        }

        vec3 gamma = vec3(1.0/2.2);

        Color = vec4(pow(outputColor, gamma), d.a);
    } else {
        Color = vec4(1, 0.5765, 0, 0.5);
    }
}
