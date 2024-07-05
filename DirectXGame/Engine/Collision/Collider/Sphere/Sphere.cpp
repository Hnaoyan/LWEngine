#include "Sphere.h"

void Sphere::Initialize(ColliderObject object)
{
	// 基底クラス
	ICollider::Initialize(object);
	// ワールド
	worldTransform_.Initialize();

}

void Sphere::Update()
{
	// 更新
	worldTransform_.UpdateMatrix();
}
