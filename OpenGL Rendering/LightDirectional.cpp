#include "LightDirectional.h"

LightDirectional::LightDirectional(vec3 color, vec3 angle)
{
	UpdateDirectionalLight(color, angle);
}

void LightDirectional::UpdateDirectionalLight(vec3 color, vec3 angle)
{
	Color = color;

	Direction = vec3(0, 0, 1);
	Direction = rotateZ(Direction, angle.z);
	Direction = rotateX(Direction, angle.x);
	Direction = rotateY(Direction, angle.y);
	Direction *= -1;
}
