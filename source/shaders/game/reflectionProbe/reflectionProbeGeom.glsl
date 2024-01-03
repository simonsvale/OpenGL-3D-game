#version 410 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 CubeSides[6];

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    for(int face = 0; face < 6;)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            vec4 GeomPos = gl_in[i].gl_Position;
            vs_out.FragPos = GeomPos.xyz;
            gl_Position = CubeSides[face] * GeomPos;
            EmitVertex();
        }    
        EndPrimitive();
        
        face++;
    }
} 