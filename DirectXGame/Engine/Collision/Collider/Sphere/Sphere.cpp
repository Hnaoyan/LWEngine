#include "Sphere.h"

void Sphere::Initialize(float radius, ColliderObject object)
{
	// クラス設定
	objectRegistry_ = object;
	// 半径
	radius_ = radius;

	// ワールド
	worldTransform_.Initialize();

}

void Sphere::Update(const Vector3& worldPosition)
{
	// 基底クラス
	ICollider::Update(worldPosition);

}
