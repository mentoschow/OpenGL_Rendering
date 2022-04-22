#pragma once
#include <glm.hpp>
#include <gtx/rotate_vector.hpp>

using namespace glm;

class LightDirectional
{
public:
	LightDirectional(vec3 position, vec3 color, vec3 angle, float strength);

	vec3 Position;
	vec3 Direction;
	vec3 Color;
	float Strength;
	bool enable;

	void UpdateDirectionalLightPos(vec3 position);
	void UpdateDirectionalLightColor(vec3 color);
	void UpdateDirectionalLightDir(vec3 angle);
	void UpdateDirectionalLightStrength(float value);
	void SetActive(bool value);

private:
};

