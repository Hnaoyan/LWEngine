#include "AABB.h"

void AABB::Initialize(ColliderShape shape)
{
	// 基底クラス
	ICollider::Initialize(shape);
	// ワールド
	worldTransform_.Initialize();

}

void AABB::Update()
{

	// 更新
	worldTransform_.UpdateMatrix();
}
