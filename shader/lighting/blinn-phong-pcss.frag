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

/////////////////// Shadow Map //////////////////////////

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

float saturate(float value)
{
    if (value > 1.) return 1.0;
    else if (value < 0.) return 0.0;

    return value;
}

/////////////////////// PCSS /////////////////////////////////

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float PCSShadowCalculation(int i)
{
    float currentDepth = length(fs_in.WorldPos - lightPos[i]);
    if (currentDepth > far_plane[i])
    {
        return 1.0;
    }

    float texelSize = 2.0 * near_plane[i] / float(textureSize(shadowCubmap[i], 0).x);
    vec3 fragToLight = normalize(fs_in.WorldPos - lightPos[i]);
    vec3 lightToFrag = -fragToLight;
    vec3 normal = normalize(fs_in.WorldNormal);
    float cosTheta = dot(lightToFrag, normal);
    float tanTheta = abs(sqrt(1 - cosTheta * cosTheta) / cosTheta);
    float biasMagicNum = 20.0;
    float bias = texelSize * tanTheta * currentDepth * biasMagicNum + 0.01;

    // calculate avg blocker depth
    // ---------------------------
    float blockerDepth = 0.0;
    float numBlockers = 0.0;
    
    int samples = 20;
    float lightSize = near_plane[i];
    float searchWidth = lightSize * (currentDepth - near_plane[i]) / currentDepth * 0.01;

    for (int k = 0; k < samples; ++k)
    {
        float shadowMapDepth = texture(shadowCubmap[i], fragToLight + sampleOffsetDirections[k] * searchWidth).r;
        shadowMapDepth *= far_plane[i];
        if (currentDepth - shadowMapDepth > bias)
        {
            numBlockers += 1.0;
            blockerDepth += shadowMapDepth;
        }
    }
    
    blockerDepth = blockerDepth / numBlockers;

    // calculate shadow
    // ----------------
    float shadow = 0.0;

    // searchWidth = minWidth[for the sharpest part] + (currentDepth - blockerDepth) / blockerDepth * incWidth
    searchWidth = 0.001 + (currentDepth - blockerDepth) / blockerDepth * 0.001;
    for (int k = 0; k < samples; ++k)
    {
        float shadowMapDepth = texture(shadowCubmap[i], fragToLight + sampleOffsetDirections[k] * searchWidth).r;
        shadowMapDepth *= far_plane[i];
        if (currentDepth - shadowMapDepth > bias)
        {
            shadow += 1.0;
        }
    }

    shadow /= float(samples);

    return shadow; 
}

const float PI = 3.141592653;

// use a different func for the filter
float PCFshadow
(
    vec3 fragToLight,
    vec3 right,
    vec3 up,
    float deltaPhi,
    float filterSize,
    float currentDepth,
    float bias,
    int i
)
{
    float shadow = 0.0;

    float nSamples = 0.0;
    float deltaTheta = 0.2 * filterSize;
    for (float phi = 0.0; phi < 2.0 * PI; phi += deltaPhi)
    {
        for (float theta = 0.0; theta < filterSize; theta += deltaTheta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * fragToLight;

            float shadowMapDepth = texture(shadowCubmap[i], sampleVec).r;
            shadowMapDepth *= far_plane[i];
            if (currentDepth - shadowMapDepth > bias)
            {
                shadow += 1.0;
            }

            nSamples += 1.0;
        }
    }

    shadow = shadow / nSamples;

    return shadow;
}

float PCSShadowCalculation2(int i)
{
    float currentDepth = length(fs_in.WorldPos - lightPos[i]);
    if (currentDepth > far_plane[i])
    {
        return 1.0;
    }

    float texelSize = 2.0 * near_plane[i] / float(textureSize(shadowCubmap[i], 0).x);
    vec3 fragToLight = normalize(fs_in.WorldPos - lightPos[i]);
    vec3 lightToFrag = -fragToLight;
    vec3 normal = normalize(fs_in.WorldNormal);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, fragToLight));
    up = normalize(cross(fragToLight, right));
    float cosTheta = dot(lightToFrag, normal);
    float tanTheta = abs(sqrt(1 - cosTheta * cosTheta) / cosTheta);
    float biasMagicNum = 20.0;
    float bias = texelSize * tanTheta * currentDepth * biasMagicNum + 0.01;

    // calculate avg blocker depth
    // ---------------------------
    float blockerDepth = 0.0;
    float numBlockers = 0.0;
    float numSamples = 0.0;

    float filterSize = 0.02;
    float deltaPhi = 0.2 * PI;
    float deltaTheta = 0.2 * filterSize;

    bool blocker = false;

    for (float phi = 0.0; phi < 2.0 * PI; phi += deltaPhi)
    {
        for (float theta = 0.0; theta < filterSize; theta += deltaTheta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * fragToLight;

            float shadowMapDepth = texture(shadowCubmap[i], sampleVec).r;
            shadowMapDepth *= far_plane[i];
            if (currentDepth - shadowMapDepth > bias)
            {
                numBlockers += 1.0;
                blockerDepth += shadowMapDepth;
                blocker = true;
            }

            numSamples += 1.0;
        }
    }
    
    blockerDepth = blockerDepth / numBlockers;

    // calculate shadow
    // ----------------
    float shadow = 0.0;

    filterSize = 0.005 + (currentDepth - blockerDepth) / blockerDepth * 0.005;
    
    if (blocker)
    {
        shadow = PCFshadow(fragToLight, right, up, deltaPhi, filterSize, currentDepth, bias, i);
    }
    
    return shadow;
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
        float shadow = PCSShadowCalculation2(i);
        lighting += ((1.0 - shadow) * (diffuse + specular));
    }

    lighting *= color;

    FragColor = vec4(lighting, 1.0);
}