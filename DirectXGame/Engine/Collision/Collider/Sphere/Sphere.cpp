#include "Sphere.h"

void Sphere::Initialize(ColliderShape shape)
{
	// 基底クラス
	ICollider::Initialize(shape);
	// ワールド
	worldTransform_.Initialize();

}

void Sphere::Update()
{
	// 更新
	worldTransform_.UpdateMatrix();
}
