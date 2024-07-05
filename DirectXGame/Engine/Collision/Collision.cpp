#include "Collision.h"

bool Collision::IsCollision(const AABB& aabb1, const AABB& aabb2)
{
    aabb1, aabb2;
    return false;
}

bool Collision::IsCollision(const AABB& aabb, const Sphere& sphere)
{
    aabb, sphere;
    return false;
}

bool Collision::IsCollision(const Sphere& sphere, const AABB& aabb)
{
    sphere, aabb;
    return false;
}

bool Collision::IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
    sphere1, sphere2;
    return false;
}
