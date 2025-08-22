#include "Rectangle2D.h"

void Rectangle2D::Initialize(ColliderObject object)
{
	ICollider2D::Initialize(object);
	min_ = Vector2(-radius_, -radius_);
	max_ = Vector2(radius_, radius_);
	size_ = Vector2(radius_, radius_);
}

void Rectangle2D::Update(const Vector2& worldPosition)
{
	// 基底
	ICollider2D::Update(worldPosition);

	// 情報更新
	center_ = worldPosition;
	min_ = {
		center_.x - size_.x,
		center_.y - size_.y
	};
	max_ = {
		center_.x + size_.x,
		center_.y + size_.y
	};

}
