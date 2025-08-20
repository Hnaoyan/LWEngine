#pragma once
#include "Engine/Math/MathLib.h"
#include "../ICollider2D.h"

class Rectangle2D : public ICollider2D
{
public:

public:
	Vector2 min_ = {};
	Vector2 max_ = {};

};
