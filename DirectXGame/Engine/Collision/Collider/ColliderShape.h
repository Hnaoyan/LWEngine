#pragma once
#include <variant>

class AABB;
class Sphere;

// 形状のまとめ
using ColliderShape = std::variant<AABB*>;