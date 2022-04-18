#pragma once
#include <glm.hpp>
#include <gtx/rotate_vector.hpp>

using namespace glm;

class LightSpot
{
public:
	LightSpot(vec3 position, vec3 direction, vec3 color, float _cosInnerphy, float _cosOutterPhy);
	vec3 Position;
	vec3 Direction;
	vec3 Color;
	float cosInnerPhy;
	float cosOutterPhy;

	bool enable;

	void UpdateLightSpot(vec3 position, vec3 direction, vec3 color, float _cosInnerphy, float _cosOutterPhy);

private:

};

