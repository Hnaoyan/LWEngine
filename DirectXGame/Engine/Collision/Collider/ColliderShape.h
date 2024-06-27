#pragma once
#include <variant>

class AABB;
class Sphere;
class Plane;
class Segment;
class Triangle;


// 形状のまとめ
using ColliderShape = std::variant<AABB*>;