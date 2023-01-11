#version 330 core

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;

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

// image based light
uniform samplerCube diffuseIrradiance;
uniform samplerCube prefilterMap;

float saturate(float value)
{
    if (value > 1.) return 1.0;
    else if (value < 0.) return 0.0;

    return value;
}

/////// BRDF ///////

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;

uniform sampler2D brdfLut;

// - Fresnel term -
// schlick's approximation
vec3 fresnel_schlick(vec3 f0, vec3 view, vec3 light)
{
    vec3 h = normalize(view + light);
    float vdh = clamp(dot(normalize(view), h), 0., 1.);
    return f0 + (vec3(1.0) - f0) * pow(1. - vdh, 5.);
}

// ue's acceleration
vec3 fresnel_ue(vec3 f0, vec3 view, vec3 light)
{
    vec3 h = normalize(view + light);
    float vdh = clamp(dot(normalize(view), h), 0., 1.);

    float power = (-5.55473 * vdh - 6.98316) * vdh;
    
    return f0 + (vec3(1.0) - f0) * pow(2., power);
}

// schlick's approximation for ibl (using vdn to subsitute vdh)
vec3 fresnel_schlick_ibl(vec3 f0, vec3 view, vec3 normal)
{
    float vdn = dot(normalize(view), normal);

    return f0 + (vec3(1.0) - f0) * pow(clamp(1. - vdn, 0.0, 1.0), 5.0);
}

// - Geometry term -
// schlick's model
float geometry(vec3 view, vec3 light, vec3 normal)
{   
    float vdn = max(dot(view, normal), 0.);
    float ldn = max(dot(light, normal), 0.);
    float roughness_ = max(roughness, 0.001);

    float k = pow(roughness_ + 1., 2.0) / 8.0;
    float g1_view = vdn / (vdn * (1 - k) + k);
    float g1_light = ldn / (ldn * (1 - k) + k);
    
    return g1_view * g1_light;
}

// Normal distribution function/term
// Beckmann's model
float ndf_beckmann(vec3 view, vec3 light, vec3 normal)
{
    vec3 h = normalize(view + light);
    float hdn = max(dot(h, normal), 0.);
    float roughness_ = max(roughness, 0.001);

    float alpha = roughness_ * roughness_;
    float alpha2 = alpha * alpha;
    float cos2_theta_h = hdn * hdn;
    float cos4_theta_h = cos2_theta_h * cos2_theta_h;
    float tan2_theta_h = (1 - cos2_theta_h) / cos2_theta_h;

    return exp(-tan2_theta_h / alpha2) / (PI * alpha2 * cos4_theta_h);
}

// GGX/Trowbridge-Reitz model
float ndf_ggxtr(vec3 view, vec3 light, vec3 normal)
{
    vec3 h = normalize(view + light);
    float hdn = max(dot(h, normal), 0.);
    float roughness_ = max(roughness, 0.001);

    float alpha = roughness_ * roughness_;
    float alpha2 = alpha * alpha;
    float cos2_theta_h = hdn * hdn;

    return alpha2 / (PI * (cos2_theta_h * (alpha2 - 1.) + 1.) * (cos2_theta_h * (alpha2 - 1.) + 1.));
}

// GTR/Generalized Trowbridge-Reitz model
// gamma = 1
float ndf_gtr1(vec3 view, vec3 light, vec3 normal)
{
    vec3 h = normalize(view + light);
    float hdn = max(dot(h, normal), 0.);
    float roughness_ = max(roughness, 0.001);

    float alpha = roughness_ * roughness_;
    float alpha2 = alpha * alpha;
    float cos2_theta_h = hdn * hdn;
    float c = (alpha2 - 1.) / (PI * log(alpha2));

    return c / (1. + (alpha2 - 1.) * cos2_theta_h);
}

// gamma = 2
float ndf_gtr2(vec3 view, vec3 light, vec3 normal)
{
    vec3 h = normalize(view + light);
    float hdn = max(dot(h, normal), 0.);
    float roughness_ = max(roughness, 0.001);

    float alpha = roughness_ * roughness_;
    float alpha2 = alpha * alpha;
    float cos2_theta_h = hdn * hdn;
    float c = alpha2 / PI;

    return c / pow(1. + (alpha2 - 1.) * cos2_theta_h, 2.);
}

/////// main ///////

void main()
{           
    // vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;

    vec3 normal = normalize(fs_in.WorldNormal);
    vec3 view = normalize(viewPos - fs_in.WorldPos);

    vec3 f0 = mix(vec3(0.04), albedo, metallic);

    ///////////////////lighting///////////////////////

    //--------------Dirction-Light--------------------
    vec3 dLight = normalize(lightDir);
    vec3 dFresnelTerm = fresnel_ue(f0, view, dLight);

    // diffuse
    vec3 diffuse = (1. - dFresnelTerm) * (1. - metallic) * albedo * (1 / PI) * max(dot(normal, dLight), 0.) * dirctLightCol * dirIrradiance;
    // specular
    float dDistributionTerm = ndf_gtr2(view, dLight, normal);
    float dGeometryTerm = geometry(view, dLight, normal);
    vec3 specular = dFresnelTerm * dDistributionTerm * dGeometryTerm * (1 / (4 * max(dot(normal, view), 0.)) + 0.0001) * dirctLightCol * dirIrradiance;
    
    vec3 Lo = diffuse + specular;


    //----------------Point-Light---------------------
    for (int i = 0; i < 2; i++)
    {
        // light
        float distance = length(fs_in.WorldPos - lightPos[i]);
        float falloff = pow(saturate(1.0 - pow((distance/far_plane[i]), 4.0)), 2.0) / (pow(distance, 2.0) + 1.0);
        vec3 pLight = normalize(lightPos[i] - fs_in.WorldPos);
        vec3 pFresnelTerm = fresnel_ue(f0, view, pLight);

        // diffuse
        diffuse = (1. - pFresnelTerm) * (1. - metallic) * albedo * (1 / PI) * max(dot(normal, pLight), 0.) * lightCol[i] * Irradiance[i] * falloff;
        // specular
        float pDistributionTerm = ndf_gtr2(view, pLight, normal);
        float pGeometryTerm = geometry(view, pLight, normal);
        specular = pFresnelTerm * pDistributionTerm * pGeometryTerm * (1 / (4 * max(dot(normal, view), 0.)) + 0.0001) * lightCol[i] * Irradiance[i] * falloff;

        Lo += diffuse + specular;
    }

    //----------------I-B-Light---------------------
    vec3 R = reflect(-view, normal);
    vec3 iFresnelTerm = fresnel_schlick_ibl(f0, view, normal);

    // diffuse
    vec3 diffIrradiance = texture(diffuseIrradiance, normal).rgb;
    diffuse = (1. - iFresnelTerm) * (1. - metallic) * albedo * (1 / PI) * diffIrradiance;

    // specular
    vec3 specIrradiance = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLut, vec2(max(dot(normal, view), 0.0), roughness)).rg;
    specular = specIrradiance * (iFresnelTerm * brdf.x + brdf.y);

    Lo += diffuse + specular;

    FragColor = vec4(Lo, 1.0);
}