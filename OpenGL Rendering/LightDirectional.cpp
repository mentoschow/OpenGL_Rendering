#include "LightDirectional.h"

LightDirectional::LightDirectional(vec3 position, vec3 color, vec3 angle, float strength)
{
	SetActive(true);
	UpdateDirectionalLightPos(position);
	UpdateDirectionalLightColor(color);
	UpdateDirectionalLightDir(angle);
	UpdateDirectionalLightStrength(strength);
}

void LightDirectional::UpdateDirectionalLightPos(vec3 position)
{
	Position = position;
}

void LightDirectional::UpdateDirectionalLightColor(vec3 color)
{
	Color = color;
}

void LightDirectional::UpdateDirectionalLightDir(vec3 angle)
{
	Direction = vec3(0, 0, 1);
	Direction = rotateZ(Direction, angle.z);
	Direction = rotateX(Direction, angle.x);
	Direction = rotateY(Direction, angle.y);
	Direction *= -1;
}

void LightDirectional::UpdateDirectionalLightStrength(float value)
{
	Strength = value;
}

void LightDirectional::SetActive(bool value)
{
	enable = value;
}
