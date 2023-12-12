#version 410 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


// Define functions:
float ShadowCalculation(vec3 fragPos, float LightDist);

// main
void main()
{           
    float constant = 1.0;
    float linear = 0.022;
    float quadratic = 0.0019;
    
    float LightDistance = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (constant + linear * LightDistance + quadratic * (LightDistance * LightDistance)); 

    vec3 normal = normalize(fs_in.Normal);
    vec3 LightColor = vec3(1.0);

    // ambient
    vec3 ambient = 0.05 * LightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * LightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
    vec3 specular = spec * LightColor;  


    diffuse *= attenuation;
    specular *= attenuation;

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPos, LightDistance);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(diffuseTexture, fs_in.TexCoords).rgb;    
    
    FragColor = vec4(lighting, 1.0);
}


// Calculate shadows
float ShadowCalculation(vec3 fragPos, float Depth)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    
    float shadow = 0.0;
    float bias = 0.20;
    int samples = 10;

    // How fuzzy the edges of the shadow should be, higher value = less fuzzy.
    float ShadowFuzzyValue = 60.0;

    //float viewDistance = length(viewPos - fragPos);
    float diskRadius = 1.0 / (ShadowFuzzyValue / (Depth * 0.2) );
    for(int i = 0; i < samples;)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]

        if(Depth - bias > closestDepth)
        {
            shadow += 1.0;
        }
        i++;
    }
    shadow /= float(samples);
        
    // display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}
