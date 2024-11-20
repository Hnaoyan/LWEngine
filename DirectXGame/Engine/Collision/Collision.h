#pragma once
#include "Collider/ColliderLists.h"

/// <summary>
/// 当たり判定関数
/// </summary>
class Collision
{
public:
	// AABB
	static bool IsCollision(const AABB& aabb1,const AABB& aabb2);
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);
	static bool IsCollision(const AABB& aabb, const OBB& obb);
	// Sphere
	static bool IsCollision(const Sphere& sphere, const AABB& aabb);
	static bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);
	static bool IsCollision(const Sphere& sphere, const OBB& obb);
	// OBB
	static bool IsCollision(const OBB& obb1, const OBB& obb2);
	static bool IsCollision(const OBB& obb, const Sphere& sphere);
	static bool IsCollision(const OBB& obb, const AABB& aabb);

};