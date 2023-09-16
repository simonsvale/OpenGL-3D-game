
#shader fragment
#version 410 core

void main()
{
    gl_FragColor = vec4(0.7f, 0.2f, 0.1f, 1.0f);
}


#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

