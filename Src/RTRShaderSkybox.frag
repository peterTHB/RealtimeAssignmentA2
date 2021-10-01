#version 460
out vec4 FragColor;

in VertexData {
    vec3 TexCoords;
} fs_in;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, fs_in.TexCoords);
}