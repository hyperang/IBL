#ifndef _light_h_
#define _light_h_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class DirectionLight
{
public:
	DirectionLight(glm::vec3 position, glm::vec3 lookat, glm::vec3 color, float irradiance);

	glm::vec3 getPosition();

	glm::vec3 getDirection();

	glm::mat4 setLightSpaceMatrix(const float& near, const float& far);

	glm::mat4 getLightSpaceMatrix();

	glm::vec3 getColor();

	float getIrradiance();

	void setColor(const glm::vec3& value);

	void setIrradiance(const float& value);

protected:
	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 color;
	float irradiance;

	glm::mat4 lightSpaceMatrix;
};

class PointLight
{
public:
	PointLight(glm::vec3& position, float range, float size, glm::vec3& color, float radiantIntensity);
	
	glm::vec3 getPosition();

	float getRange();

	std::vector<glm::mat4> getLightSpaceMatrix(const float& aspect);

	glm::vec3 getColor();

	float getIrradiance();

	float getSize();

	void setColor(const glm::vec3& value);

	void setIrradiance(const float& value);

	void setRange(const float& value);

	void setPosition(const glm::vec3& value);

protected:
	glm::vec3 position;
	float size;
	float range;

	glm::vec3 color;
	float irradiance;
};

#endif // !_light_h_
