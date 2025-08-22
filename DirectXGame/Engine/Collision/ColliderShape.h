#pragma once
#include <variant>
#include "2D/Collider2DLists.h"

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

using Collider2DShape = std::variant<Rectangle2D>;
