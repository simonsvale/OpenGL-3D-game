// Fragment shader // Rework later
#version 410 core

// output
out vec4 FragColor;

struct Material {
    sampler2D DiffuseMap;
    sampler2D SpecularMap;
    float ShineValue;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;

    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct GlobalDirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Should be a uniform passed to the shader, change later...
//#define PointLightAmount 2
//#define SpotLightAmount 2

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

#define POINT_LIGHT_AMOUNT 2
#define SPOT_LIGHT_AMOUNT 2

uniform PointLight PointLightArr[POINT_LIGHT_AMOUNT];
uniform SpotLight SpotLightArr[SPOT_LIGHT_AMOUNT];

uniform GlobalDirectionalLight GlobalLight;
uniform Material material;

// Define functions:
vec3 CalculateGlobalDirectionalLight(GlobalDirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Define the output vec3 result.
    vec3 result = CalculateGlobalDirectionalLight( GlobalLight, norm, viewDir);

    // Calculate the values for all point lights.
    for(int PointLightNumber = 0; PointLightNumber < POINT_LIGHT_AMOUNT;)
    {
        result += CalculatePointLight( PointLightArr[PointLightNumber], norm, FragPos, viewDir );    
        PointLightNumber++;
    }

    // Calculate the values for all spot lights.
    for(int SpotLightNumber = 0; SpotLightNumber < SPOT_LIGHT_AMOUNT;)
    {
        result += CalculateSpotLight( SpotLightArr[SpotLightNumber], norm, FragPos, viewDir ); 
        SpotLightNumber++;
    }
    
    // return the frag color of the shader.
    FragColor = vec4(result, 1.0);
}

// Done, make better later
vec3 CalculateGlobalDirectionalLight(GlobalDirectionalLight light, vec3 normal, vec3 viewDir)
{
    // ambient
    vec3 ambient = light.ambient * texture(material.DiffuseMap, TexCoords).rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.DiffuseMap, TexCoords).rgb;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ShineValue);
    vec3 specular = light.specular * spec * texture(material.SpecularMap, TexCoords).rgb;  
        
    return (ambient + diffuse + specular);
}

// Calculates light values for the point light.
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ambient
    vec3 ambient = light.ambient * texture(material.DiffuseMap, TexCoords).rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.DiffuseMap, TexCoords).rgb;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ShineValue);
    vec3 specular = light.specular * spec * texture(material.SpecularMap, TexCoords).rgb;  

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    ambient *= attenuation;  
    diffuse *= attenuation;
    specular *= attenuation;
        
    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ambient
    vec3 ambient = light.ambient * texture(material.DiffuseMap, TexCoords).rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.DiffuseMap, TexCoords).rgb;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ShineValue);
    vec3 specular = light.specular * spec * texture(material.SpecularMap, TexCoords).rgb;  

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Atunation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    ambient *= attenuation;  
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
        
    return (ambient + diffuse + specular);
}