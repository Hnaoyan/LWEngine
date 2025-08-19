#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class PenetrationResolver
{
public:
	static Vector3 ExtrusionCalculation(AABB from, AABB to);
	Vector3 ExtrusionPushPower(AABB from, AABB to);
};
