#pragma once
#include <variant>

class AABB;
class OBB;
class Sphere;
class Plane;
class Segment;
class Triangle;


// 形状のまとめ
using ColliderShape = std::variant<AABB*, OBB*, Sphere*>;