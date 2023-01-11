#version 330 core

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float roughness;
uniform float resolution;

const float PI = 3.14159265359;

/////// Normal Distribution Function ///////

float ndf_ggxtr(vec3 normal, vec3 half, float roughness)
{
    float hdn = max(dot(half, normal), 0.);

    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float cos2_theta_h = hdn * hdn;

    return alpha2 / (PI * (cos2_theta_h * (alpha2 - 1.) + 1.) * (cos2_theta_h * (alpha2 - 1.) + 1.));
}

float ndf_gtr1(vec3 view, vec3 light, vec3 normal)
{
    vec3 h = normalize(view + light);
    float hdn = max(dot(h, normal), 0.);

    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float cos2_theta_h = hdn * hdn;
    float c = (alpha2 - 1.) / (PI * log(alpha2));

    return c / (1. + (alpha2 - 1.) * cos2_theta_h);
}

float ndf_gtr2(vec3 view, vec3 light, vec3 normal)
{
    vec3 h = normalize(view + light);
    float hdn = max(dot(h, normal), 0.);

    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float cos2_theta_h = hdn * hdn;
    float c = alpha2 / PI;

    return c / pow(1. + (alpha2 - 1.) * cos2_theta_h, 2.);
}

/////// Low Discrepancy Sequence ///////

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
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

// no bit options version
float VdCNoBitOps(uint n, uint base)
{
    float invBase = 1.0 / float(base);
    float denom   = 1.0;
    float result  = 0.0;

    for(uint i = 0u; i < 32u; ++i)
    {
        if(n > 0u)
        {
            denom   = mod(float(n), 2.0);
            result += denom * invBase;
            invBase = invBase / 2.0;
            n       = uint(float(n) / 2.0);
        }
    }

    return result;
}

vec2 HammersleyNoBitOps(uint i, uint N)
{
    return vec2(float(i)/float(N), VdCNoBitOps(i, 2u));
}

/////// Importance Sample ///////

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float alpha = roughness * roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (alpha * alpha - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
	// from spherical coordinates to cartesian coordinates - halfway vector
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	// from tangent-space H vector to world-space sample vector
	vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

vec3 ImportanceSampleGTR1(vec2 Xi, vec3 N, float roughness)
{
    float alpha = roughness * roughness;
    
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - pow(alpha * alpha, 1 - Xi.y))) / (1.0 - (alpha * alpha));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    
    // from spherical coordinates to cartesian coordinates - halfway vector
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
    
    // from tangent-space H vector to world-space sample vector
    vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

vec3 ImportanceSampleGTR2(vec2 Xi, vec3 N, float roughness)
{
    return ImportanceSampleGGX(Xi, N, roughness);
}

/////// main ///////

void main()
{		
    vec3 N = normalize(WorldPos);
    
    // make the simplyfying assumption that V equals R equals the normal 
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;
    
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            // sample from the environment's mip level based on roughness/pdf
            float D   = ndf_ggxtr(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution); // resolution of source cubemap (per face)
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
            
            prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
}