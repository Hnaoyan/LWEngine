#pragma once
#include <variant>

//---3D用---//
class AABB;
class OBB;
class Sphere;
class Plane;
class Segment;
class Triangle;

// 形状のまとめ
using ColliderShape = std::variant<AABB*, OBB*, Sphere*>;

//---2D用---//
class Rectangle2D;
class Circle2D;

using Collider2DShape = std::variant<Rectangle2D*>;
