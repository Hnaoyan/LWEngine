#pragma once
#include "../ICollider.h"

/// <summary>
/// AABB
/// </summary>
class AABB : public ICollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	void Initialize(const Vector3& radius, ColliderObject object);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& worldPosition) override;

public:
	void SetRadius(const Vector3& radius) {
		radius_ = radius;
	}

public:
	Vector3 min_ = {};	// 最小
	Vector3 max_ = {};	// 最大

private:
	Vector3 radius_ = {};	// 半径サイズ
	
};
