#include "LightPoint.h"

LightPoint::LightPoint(vec3 position, vec3 color, float attenuation)
{
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;

	UpdateLightPoint(position, color, attenuation);
}

void LightPoint::UpdateLightPoint(vec3 position, vec3 color, float attenuation)
{
	Position = position;
	Color = color;
	Attenuation = attenuation;
}
