#pragma once
#include "../ICollider.h"

/// <summary>
/// 球体
/// </summary>
class Sphere : public ICollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	void Initialize(float radius, ColliderObject object);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& worldPosition) override;

public:
	// サイズ
	float radius_ = 0.0f;

};
