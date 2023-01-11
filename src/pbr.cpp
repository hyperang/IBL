#include <pbr.h>

MetalnessSettings::MetalnessSettings(glm::vec3 a, float m, float r) : albedo(a), metallic(m), roughness(r)
{
	this->useTexture = false;
	this->albedoTexturePath = nullptr;
	this->metallicTexturePath = nullptr;
	this->roughnessTexturePath = nullptr;
}

glm::vec3 MetalnessSettings::getAlbedo()
{
	return this->albedo;
}

float MetalnessSettings::getMetallic()
{
	return this->metallic;
}

float MetalnessSettings::getRoughness()
{
	return this->roughness;
}

void MetalnessSettings::setAlbedo(glm::vec3& value)
{
	this->albedo = value;
}

void MetalnessSettings::setMetallic(float& value)
{
	this->metallic = value;
}

void MetalnessSettings::setRoughness(float& value)
{
	this->roughness = value;
}


MicrofacetSettings::MicrofacetSettings(glm::vec3 f, float r) : f0(f), roughness(r)
{

}

glm::vec3 MicrofacetSettings::getF0()
{
	return this->f0;
}

float MicrofacetSettings::getRoughness()
{
	return this->roughness;
}

void MicrofacetSettings::setF0(glm::vec3& value)
{
	this->f0 = value;
}

void MicrofacetSettings::setRoughness(float& value)
{
	this->roughness = value;
}