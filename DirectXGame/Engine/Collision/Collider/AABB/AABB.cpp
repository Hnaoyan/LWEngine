#include "AABB.h"

void AABB::Initialize(const Vector3& radius, ColliderObject object)
{
	// クラス設定
	objectRegistry_ = object;
	// 半径設定
	radius_ = radius;
	// ワールド
	worldTransform_.Initialize();

}

void AABB::Update(const Vector3& worldPosition)
{
	// 基底クラス
	ICollider::Update(worldPosition);

	// 衝突データ
	min_ = {
		worldTransform_.GetWorldPosition().x - radius_.x,
		worldTransform_.GetWorldPosition().y - radius_.y,
		worldTransform_.GetWorldPosition().z - radius_.z
	};

	max_ = {
		worldTransform_.GetWorldPosition().x + radius_.x,
		worldTransform_.GetWorldPosition().y + radius_.y,
		worldTransform_.GetWorldPosition().z + radius_.z
	};

}
