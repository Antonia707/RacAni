#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 FragPos[];
in vec3 Normal[];
in vec2 TexCoords[];
in vec4 FragPosLightSpace[];

out vec3 gFragPos;
out vec3 gNormal;
out vec2 gTexCoords;
out vec4 gFragPosLightSpace;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gFragPos = FragPos[i];
        gNormal = Normal[i];
        gTexCoords = TexCoords[i];
        gFragPosLightSpace = FragPosLightSpace[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
