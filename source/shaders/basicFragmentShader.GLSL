// Fragment shader
#version 410 core

out vec4 FragColor; 
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{   
    // Set the fragcolor
    FragColor = texture(tex0, texCoord);
}
