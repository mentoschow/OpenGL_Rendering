#pragma once
#include <glm.hpp>
#include <gtx/rotate_vector.hpp>

using namespace glm;

class LightPoint
{
public:
	LightPoint(vec3 position, vec3 color);
	vec3 Position;
	vec3 Color;

	float constant;
	float linear;
	float quadratic;

	bool enable;

	void UpdateLightPoint(vec3 position, vec3 color);

private:

};

