#version 410 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 diffuse_map;
layout (location = 2) in vec3 normal_map;
layout (location = 3) in vec2 roughness_map;
layout (location = 4) in vec2 metallic_map;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{

} 