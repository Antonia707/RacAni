#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eyePosition; // camera world position

out vec2 TexCoord;

vec3 rotateAxisAngle(vec3 v, vec3 axis, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return v * c + cross(axis, v) * s + axis * dot(axis, v) * (1.0 - c);
}

void main()
{
    // Quad center in world space
    vec3 center = vec3(model[3]);

    // Initial normal
    vec3 n0 = vec3(0.0, 0.0, 1.0);

    // Direction from quad to camera
    vec3 toCamera = normalize(eyePosition - center);

    // Compute rotation axis and angle
    vec3 axis = cross(n0, toCamera);
    float cosAngle = clamp(dot(n0, toCamera), -1.0, 1.0);
    float angle = acos(cosAngle);

    // Rotate vertex if needed
    vec3 pos = aPos;
    if (length(axis) > 0.0001)
    {
        axis = normalize(axis);
        pos = rotateAxisAngle(aPos, axis, angle);
    }

    vec3 rotatedPos = vec3(-pos.z, pos.y, pos.x); // +90° Z rotation
    vec3 worldPos = center + rotatedPos;

    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(worldPos, 1.0);
}
