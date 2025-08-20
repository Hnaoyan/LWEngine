#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/Collision/2D/Collider2DLists.h"

class PenetrationResolver
{
public:
	static Vector3 ExtrusionCalculation(AABB mover, AABB obstacle);
	// 
	static Vector2 Extrusion2DCalculation(Rectangle2D mover, Rectangle2D obstacle);
	Vector3 ExtrusionPushPower(AABB from, AABB to);
};
