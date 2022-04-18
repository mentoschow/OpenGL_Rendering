#include "LightPoint.h"

LightPoint::LightPoint(vec3 position, vec3 color)
{
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;

	enable = true;

	UpdateLightPoint(position, color);
}

void LightPoint::UpdateLightPoint(vec3 position, vec3 color)
{
	Position = position;
	Color = color;
}
