#version 460
out vec4 FragColor;

in VertexData {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform vec3 u_CameraPos;
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(fs_in.FragPos - u_CameraPos);
    vec3 R = reflect(I, normalize(fs_in.Normal));

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}

