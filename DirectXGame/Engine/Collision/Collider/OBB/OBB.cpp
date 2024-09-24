#include "OBB.h"

float OBB::LengthSeparateAxis(const Vector3& separate, const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = std::fabsf(Vector3::Dot(separate, v1));
	float r2 = std::fabsf(Vector3::Dot(separate, v2));
	float r3 = 0.0f;
	// 参照が有効な場合、Dot計算を行う
	r3 = std::fabsf(Vector3::Dot(separate, v3));
	return r1 + r2 + r3;
}

void OBB::Initialize(const Vector3& radius, ColliderObject object)
{
	//
	objectRegistry_ = object;
	//
	length_ = radius;
	//
	worldTransform_.Initialize();

	// Axis
	arbitraryAxis[0] = { 1.0f,0.0f,0.0f };
	arbitraryAxis[1] = { 0.0f,1.0f,0.0f };
	arbitraryAxis[2] = { 0.0f,0.0f,1.0f };
}

void OBB::Update(const Vector3& worldPosition)
{
	// 基底
	ICollider::Update(worldPosition);

	// 
	position_ = worldPosition;
}
