#pragma once
#include "Collider/ColliderLists.h"

class Collision
{
public:

	static bool IsCollision(const AABB& aabb1,const AABB& aabb2);
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);
	static bool IsCollision(const Sphere& sphere, const AABB& aabb);
	static bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);
};