// Fragment shader
#version 410 core

out vec4 FragColor;  
in vec2 texCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D tex0;

void main()
{
    // FragColor = texture(tex0, texCoord) * vec4(lightColor * objectColor, 1.0);
    FragColor = vec4(lightColor * objectColor, 1.0);
}
