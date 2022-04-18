#include "LightSpot.h"

LightSpot::LightSpot(vec3 position, vec3 angle, vec3 color, float _cosInnerphy, float _cosOutterPhy)
{
	enable = true;
	UpdateLightSpot(position, angle, color, _cosInnerphy, _cosOutterPhy);
}

void LightSpot::UpdateLightSpot(vec3 position, vec3 angle, vec3 color, float _cosInnerphy, float _cosOutterPhy)
{
	Position = position;
	Color = color;
	cosInnerPhy = _cosInnerphy;
	cosOutterPhy = _cosOutterPhy;

	Direction = vec3(0, 0, 1);
	Direction = rotateZ(Direction, angle.z);
	Direction = rotateX(Direction, angle.x);
	Direction = rotateY(Direction, angle.y);
	Direction *= -1;
}
