#include "AABB.h"

void AABB::Initialize(ColliderObject object)
{
	// 基底クラス
	ICollider::Initialize(object);
	// ワールド
	worldTransform_.Initialize();

}

void AABB::Update()
{

	// 更新
	worldTransform_.UpdateMatrix();
}
