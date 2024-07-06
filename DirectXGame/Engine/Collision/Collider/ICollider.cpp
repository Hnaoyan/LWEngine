#include "ICollider.h"

void ICollider::Initialize(ColliderObject object)
{
	objectRegistry_ = object;
}

void ICollider::Update(const Vector3& worldPosition)
{
	// 座標更新
	worldTransform_.transform_.translate = worldPosition;
	// 更新
	worldTransform_.UpdateMatrix();

}
