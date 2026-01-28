#version 330 core

in vec3 gFragPos;
in vec3 gNormal;
in vec2 gTexCoords;
in vec4 gFragPosLightSpace;

out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightAmbient;
uniform vec3 eyePosition;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 lightDirection;
uniform float cutOff;
uniform float outerCutOff;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.005 * (1.0 - dot(normalize(gNormal), normalize(lightPosition - gFragPos))), 0.001);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    shadow = smoothstep(0.0, 1.0, shadow);

    if (projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}

void main() {
    vec3 norm = normalize(gNormal);
    vec3 lightDir = normalize(lightPosition - gFragPos);
    vec3 viewDir = normalize(eyePosition - gFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

    vec3 texColor = texture(diffuseTexture, gTexCoords).rgb;

    vec3 ambient = lightAmbient * texColor;
    vec3 diffuse = diff * lightColor * texColor;
    vec3 specular = spec * lightColor * materialSpecular;

    float theta = dot(lightDir, normalize(-lightDirection));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

    float shadow = ShadowCalculation(gFragPosLightSpace);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular) * intensity;

    FragColor = vec4(lighting, 1.0);
}

