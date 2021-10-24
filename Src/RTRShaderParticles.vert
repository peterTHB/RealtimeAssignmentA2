#version 460
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out VertexData {
    vec2 TexCoords;
	vec4 ParticleColor;
} vs_out;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;
uniform vec3 u_Offset;
uniform vec4 u_Color;
uniform float u_Scale;

void main() {
	vs_out.TexCoords = a_TexCoord;
	vs_out.ParticleColor = u_Color;
	gl_Position = u_Projection * u_ViewMatrix * vec4((a_Position * u_Scale) + u_Offset, 1.0);
}