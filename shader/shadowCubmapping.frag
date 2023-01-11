#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 WorldPos;
    vec3 WorldNormal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform samplerCube shadowCubmap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float far_plane;

float ShadowCalculation(vec3 WorldPos, vec3 lightPos)
{
    // Get vector between fragment position and light position
    vec3 fragDir = WorldPos - lightPos;
    // Use the light to fragment vector to sample from the depth cubmap
    float closestDepth = texture(shadowCubmap, fragDir).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // Get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragDir);
    if (currentDepth > far_plane) return 1.0;
    // Test for shadows
    float shadow = currentDepth - closestDepth > 0.1 ? 1.0 : 0.0;
    //float shadow = closestDepth;

    return shadow;
}

float saturate(float value)
{
    if (value > 1.) return 1.0;
    else if (value < 0.) return 0.0;

    return value;
}

void main()
{           
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.WorldNormal);
    vec3 lightColor = vec3(1.0);
    // falloff
    //float distance = length(fs_in.WorldPos - lightPos);
    //float falloff = pow(saturate(1. - pow((distance/far_plane), 4.)), 2.) / (pow(distance, 2.) + 1);
    //lightColor *= falloff;
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.WorldPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.WorldPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.WorldPos, lightPos);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
    //FragColor = vec4(shadow, shadow, shadow, 1.0);
}