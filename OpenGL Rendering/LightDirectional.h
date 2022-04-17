#pragma once
#include <glm.hpp>
#include <gtx/rotate_vector.hpp>

using namespace glm;

class LightDirectional
{
public:
	LightDirectional(vec3 color, vec3 angle);

	vec3 Position;
	vec3 Direction;
	vec3 Color;

	void UpdateDirectionalLight(vec3 color, vec3 angle);

private:
};

