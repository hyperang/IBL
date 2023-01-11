#include <light.h>

// Direction Light
DirectionLight::DirectionLight(glm::vec3 position, glm::vec3 lookat, glm::vec3 color, float irradiance) : position(position), color(color), irradiance(irradiance)
{
	direction = glm::normalize(lookat - position);
}

glm::vec3 DirectionLight::getPosition()
{
	return this->position;
}

glm::vec3 DirectionLight::getDirection()
{
	return this->direction;
}

glm::mat4 DirectionLight::setLightSpaceMatrix(const float& near, const float& far)
{
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
	glm::mat4 lightView = glm::lookAt(this->position, this->position + this->direction, glm::vec3(0.0f, 1.0f, 0.0f));

	this->lightSpaceMatrix = lightProjection * lightView;

	return this->lightSpaceMatrix;
}

glm::mat4 DirectionLight::getLightSpaceMatrix()
{
	return this->lightSpaceMatrix;
}

glm::vec3 DirectionLight::getColor()
{
	return this->color;
}

float DirectionLight::getIrradiance()
{
	return this->irradiance;
}

void DirectionLight::setColor(const glm::vec3& value)
{
	this->color = value;
}

void DirectionLight::setIrradiance(const float& value)
{
	this->irradiance = value;
}


// Point Light
PointLight::PointLight(glm::vec3& position, float range, float size, glm::vec3& color, float irradiance) : position(position), range(range), color(color), irradiance(irradiance), size(size)
{

}

glm::vec3 PointLight::getPosition()
{
	return this->position;
}

float PointLight::getRange()
{
	return this->range;
}

std::vector<glm::mat4> PointLight::getLightSpaceMatrix(const float& aspect)
{
	std::vector<glm::mat4> lightSpaceMatrixs;

	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), aspect, size, this->getRange());

	lightSpaceMatrixs.push_back(lightProjection * glm::lookAt(this->position, this->position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightSpaceMatrixs.push_back(lightProjection * glm::lookAt(this->position, this->position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightSpaceMatrixs.push_back(lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightSpaceMatrixs.push_back(lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	lightSpaceMatrixs.push_back(lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightSpaceMatrixs.push_back(lightProjection * glm::lookAt(this->position, this->position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightSpaceMatrixs;
}

glm::vec3 PointLight::getColor()
{
	return this->color;
}

float PointLight::getIrradiance()
{
	return this->irradiance;
}

void PointLight::setColor(const glm::vec3& value)
{
	this->color = value;
}

void PointLight::setIrradiance(const float& value)
{
	this->irradiance = value;
}

void PointLight::setRange(const float& value)
{
	this->range = value;
}

void PointLight::setPosition(const glm::vec3& value)
{
	this->position = value;
}

float PointLight::getSize()
{
	return this->size;
}