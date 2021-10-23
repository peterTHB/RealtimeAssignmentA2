#version 460
layout (location = 0) in vec3 a_Position;

out VertexData {
    vec3 FragPos;
} vs_out;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main() {
    vs_out.FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_out.FragPos, 1.0);
}