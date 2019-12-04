#version 100

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

        for (int i = 0; i < 6; i++) {
            if (i == uLighting.pointLightCount) {
                break;
            }

            vec3 toLight = normalize(uLighting.pointLights[i].position - vertPosition);
            vec3 toCamera = normalize(uCameraPosition - vertPosition);
            vec3 reflection = reflect(-toLight, normal);

            vec4 diffuse;
            if (uMaterial.textured > 0) {
                diffuse = texture2D(uSampler, vertTexcoord);
            } else {
                diffuse = uMaterial.diffuse;
            }
            vec4 pointLightColor = vec4(uLighting.pointLights[i].color, 1.0);
            vec4 diffuseComponent = pointLightColor * diffuse * max(dot(normal, toLight), 0.0);
            vec4 specularComponent = pointLightColor * uMaterial.specular * pow(max(dot(reflection, toCamera), 0.0), uMaterial.shininess);

            float distanceToLight = length(uLighting.pointLights[i].position - vertPosition);
            float attenuation = 1.0 / (uLighting.pointLights[i].constantAttenuation + (uLighting.pointLights[i].linearAttenuation + uLighting.pointLights[i].quadraticAttenuation * distanceToLight) * distanceToLight);

            vec4 colorContribution = attenuation * (uMaterial.emission + uMaterial.ambient + diffuseComponent + specularComponent);

            float gamma = 1.0 / 2.2;
            colorContribution.r = pow(colorContribution.r, gamma);
            colorContribution.g = pow(colorContribution.g, gamma);
            colorContribution.b = pow(colorContribution.b, gamma);

            outputColor += colorContribution;
        }
    } else {
        outputColor = vec4(1, 0.5765, 0, 0.5);
    }

    gl_FragColor = outputColor;
}
