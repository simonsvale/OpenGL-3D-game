// Fragment shader // Rework later
#version 410 core

out vec4 FragColor;  

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

uniform sampler2D tex0;

// Struct for material attributes.
struct Material
{
    vec3 AmbientStrength;
    vec3 DiffuseStrength;
    vec3 SpecularStrength;
    float ShineValue;
};

// Init struct
uniform Material material;


void main()
{
    // ambient
    vec3 ambient = material.AmbientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.DiffuseStrength * lightColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ShineValue); 
    vec3 specular = material.SpecularStrength * spec * lightColor;  

    // Final
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}  