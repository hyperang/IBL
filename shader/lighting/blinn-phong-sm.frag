#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 WorldPos;
    vec3 WorldNormal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;

// direction light
uniform sampler2D shadowMap;
uniform vec3 lightDir;
uniform vec3 dirctLightCol;
uniform float dirIrradiance;

// point light
uniform samplerCube shadowCubmap[2];
uniform vec3 lightPos[2];
uniform vec3 lightCol[2];
uniform float Irradiance[2];
uniform float far_plane[2];
uniform float near_plane[2];

////////////// Shadow Map //////////////////////////

float DirctLightShadowCalculation()
{
    // perform perspective divide
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = (currentDepth - closestDepth) > 0.01 ? 1.0 : 0.0;
    if (currentDepth > 1.0) shadow = 0.0;

    return shadow;
}

float PointLightShadowCalculation(int i)
{
    // Get vector between fragment position and light position
    vec3 fragDir = fs_in.WorldPos - lightPos[i];
    // Use the light to fragment vector to sample from the depth cubmap
    float closestDepth = texture(shadowCubmap[i], fragDir).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane[i];
    // Get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragDir);
    // Test for shadows
    float shadow = (currentDepth - closestDepth) > 0.1 ? 1.0 : 0.0;
    if (currentDepth > far_plane[i]) return 1.0;

    return shadow;
}

float saturate(float value)
{
    if (value > 1.) return 1.0;
    else if (value < 0.) return 0.0;

    return value;
}

///////////////// main ///////////////////////

void main()
{           
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.WorldNormal);
    
    ///////////////////lighting///////////////////////

    //--------------Dirction-Light--------------------
    // ambient
    vec3 ambient = 0.01 * dirctLightCol * dirIrradiance;
    // diffuse
    float diff = max(dot(-lightDir, normal), 0.0);
    vec3 diffuse = diff * dirctLightCol * dirIrradiance;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(-lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * dirctLightCol * dirIrradiance;
    float shadow = DirctLightShadowCalculation();
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));


    //----------------Point-Light---------------------
    for (int i = 0; i < 2; i++)
    {
        // light
        float distance = length(fs_in.WorldPos - lightPos[i]);
        float falloff = pow(saturate(1.0 - pow((distance/far_plane[i]), 4.0)), 2.0) / (pow(distance, 2.0) + 1.0);
        vec3 lightColor = lightCol[i] * Irradiance[i] * falloff;
        // diffuse
        vec3 lightDir = normalize(lightPos[i] - fs_in.WorldPos);
        float diff = max(dot(lightDir, normal), 0.0);
        diffuse = diff * lightColor;
        // specular
        halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
        specular = spec * lightColor;
        // shadow
        float shadow = PointLightShadowCalculation(i);
        lighting += ((1.0 - shadow) * (diffuse + specular));
    }

    lighting *= color;

    FragColor = vec4(lighting, 1.0);
}