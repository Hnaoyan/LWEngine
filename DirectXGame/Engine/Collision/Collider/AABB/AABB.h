#pragma once
#include "../ICollider.h"

class AABB : public ICollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	void Initialize(ColliderShape shape) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	// 半径
	Vector3 radius_ = {};

};
