#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class PenetrationResolver
{
public:
	static Vector3 ExtrusionCalculation(AABB mover, AABB obstacle);
	Vector3 ExtrusionPushPower(AABB from, AABB to);
};
