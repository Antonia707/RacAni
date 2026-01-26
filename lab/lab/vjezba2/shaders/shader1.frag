#version 330 core

in vec3 gs_FragPos;
in vec3 gs_Normal;
in vec2 gs_TexCoord;

out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightAmbient;
uniform vec3 eyePosition;

uniform sampler2D diffuseTexture;

uniform vec3 materialSpecular;
uniform float materialShininess;

void main() {
    vec3 norm = normalize(gs_Normal);
    vec3 lightDir = normalize(lightPosition - gs_FragPos);
    vec3 viewDir = normalize(eyePosition - gs_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

    vec3 texColor = texture(diffuseTexture, gs_TexCoord).rgb;

    vec3 ambient = lightAmbient * texColor;
    vec3 diffuse = diff * lightColor * texColor;
    vec3 specular = spec * lightColor * materialSpecular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
