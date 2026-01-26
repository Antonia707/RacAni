#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 FragPos[];
in vec3 Normal[];
in vec2 TexCoord[];

out vec3 gs_FragPos;
out vec3 gs_Normal;
out vec2 gs_TexCoord;

void main() {
    for (int i = 0; i < 3; ++i) {
        gs_FragPos = FragPos[i];
        gs_Normal = Normal[i];
        gs_TexCoord = TexCoord[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
