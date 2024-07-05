#pragma once
#include "../ICollider.h"

class AABB : public ICollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	void Initialize(ColliderObject object) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

public:
	// サイズ
	Vector3 min_;
	Vector3 max_;
	
};
