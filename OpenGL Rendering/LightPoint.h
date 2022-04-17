#pragma once
#include <glm.hpp>
#include <gtx/rotate_vector.hpp>

using namespace glm;

class LightPoint
{
public:
	LightPoint(vec3 position, vec3 color, float attenuation);
	vec3 Position;
	vec3 Color;
	float Attenuation;

	float constant;
	float linear;
	float quadratic;

	void UpdateLightPoint(vec3 position, vec3 color, float attenuation);

private:

};

