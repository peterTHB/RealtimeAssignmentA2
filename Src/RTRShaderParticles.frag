#version 460
out vec4 Color;

in VertexData {
    vec2 TexCoords;
	vec4 ParticleColor;
} fs_in;

uniform sampler2D u_Texture;

void main() {
	Color = (texture(u_Texture, fs_in.TexCoords) * fs_in.ParticleColor);
}