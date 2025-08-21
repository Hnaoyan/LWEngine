#pragma once
#include "Engine/Math/MathLib.h"
#include "../ICollider2D.h"

class Rectangle2D : public ICollider2D
{
public:
	void Initialize(ColliderObject object) override;
	void Update(const Vector2& worldPosition) override;
public:
	Vector2 min_ = {};
	Vector2 max_ = {};
	Vector2 center_ = {};
	float radius_ = 1.0f;
};
