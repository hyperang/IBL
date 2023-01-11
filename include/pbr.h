#ifndef _pbr_h_
#define _pbr_h_

#include <glm/glm.hpp>

class MetalnessSettings
{
public:
	MetalnessSettings(glm::vec3 a, float m, float r);

	glm::vec3 getAlbedo();

	float getMetallic();

	float getRoughness();

	void setAlbedo(glm::vec3& value);

	void setMetallic(float& value);

	void setRoughness(float& value);

protected:
	glm::vec3 albedo;
	float metallic;
	float roughness;

	bool useTexture;

	char* albedoTexturePath;
	char* metallicTexturePath;
	char* roughnessTexturePath;
};

class MicrofacetSettings
{
public:
	MicrofacetSettings(glm::vec3 f, float r);

	glm::vec3 getF0();

	float getRoughness();

	void setF0(glm::vec3& value);

	void setRoughness(float& value);

protected:
	glm::vec3 f0;
	float roughness;
};

#endif // !_pbr_h_
