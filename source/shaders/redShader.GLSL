// Fragment shader

#version 410 core

out vec4 FragColor;  
in vec3 ourColor;
in vec2 texCoord;
uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, texCoord) * vec4(1.0, 0.0, 0.3, 0.4);
}