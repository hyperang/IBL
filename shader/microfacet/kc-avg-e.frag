#version 330 core

out vec3 FragColor;

in vec2 TexCoords;

uniform vec3 f0;
uniform sampler2D brdfLut;

/////// Low Discrepancy Sequence ///////

float RadicalInverse_VdC(uint bits)
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

/////// Integration ///////

vec3 IntegrateBRDF(float roughness)
{
	vec3 res = vec3(0.0);

    const uint SAMPLE_COUNT = 1024u;
    float mu = 0.0;
    float delta = 1.0 / float(SAMPLE_COUNT);
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        float NdotV = sqrt(1 - mu * mu);
        vec2 brdf = texture(brdfLut, vec2(max(NdotV, 0.0), roughness)).rg;
        vec3 L = f0 * brdf.x + brdf.y;
        mu += delta;

        res += L * mu;
    }
    
    res /= float(SAMPLE_COUNT);
    
    return res;
}

/////// main ///////

void main()
{
    vec3 integratedBRDF = IntegrateBRDF(TexCoords.x);

    FragColor = integratedBRDF;
}