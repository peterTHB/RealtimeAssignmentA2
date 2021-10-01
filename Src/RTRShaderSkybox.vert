#version 460
layout (location = 0) in vec3 a_Position;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out VertexData {
    vec3 TexCoords;
} vs_out;

void main() {
    vs_out.TexCoords = a_Position;
    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}