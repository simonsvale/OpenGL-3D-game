#version 410 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.5, 0.1, 0.5, 255.0);
}