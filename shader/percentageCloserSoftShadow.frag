#version 330 core

const float PI = 3.1415926535897;

out vec4 FragColor;

in VS_OUT {
    vec3 WorldPos;
    vec3 WorldNormal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int shadowMapWidth;
uniform int shadowMapHeight;

bool calcBlockerDepth
(
out float avgBlockerDepth,
int filterSize,
vec4 fragPosLightSpace,
sampler2D shadowMap
)
{
    int blockerNum = 0;
    int fSize = filterSize;
    avgBlockerDepth = 0.0;

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    vec2 uv = projCoords.xy;
    int xBegin = max(0, uv.x - fSize);
    int xEnd = min(shadowMapWidth - 1, uv.x + fSize);
    int yBegin = max(0, uv.y - fSize);
    int yEnd = min(shadowMapHeight - 1, uv.y + fSize);

    float currentDepth = projCoords.z;

    for (int i = xBegin; i <= xEnd; i++)
    {
        for (int j = yBegin; j <= jEnd; j++)
        {
            float closestDepth = texture(shadowMap, vec2(i, j)).r;
            if (currentDepth - closestDepth > 0.01)
            {
                blockerNum += 1;
                avgBlockerDepth += closestDepth;
            }
        }
    }

    avgBlockerDepth = avgBlockerDepth / (blockerNum + 0.0001);

    if (avgBlockerDepth > 0.0) return true;
    return false;
}

int calcFilterSize
(
vec3 WorldPos,
vec3 lightPos,
float lightSize,
float nearPlaneDist
)
{
    float dist = distance(WorldPos, lightPos);
    float filterSize = lightSize * (dist - nearPlaneDist) / dist;


}