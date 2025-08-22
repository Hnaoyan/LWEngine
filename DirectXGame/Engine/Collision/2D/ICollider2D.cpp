#include "ICollider2D.h"

void ICollider2D::Initialize(ColliderObject object)
{
	// オブジェクトの設定
	objectRegistry_ = object;

	worldTransform_.Initialize();
}

void ICollider2D::Update(const Vector2& worldPosition)
{
	// 座標
	worldTransform_.transform_.translate = Vector3(worldPosition.x, worldPosition.y, 0.0f);
	// 更新
	worldTransform_.UpdateMatrix();
}


