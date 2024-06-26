#pragma once
#include "../../3D/WorldTransform.h"
#include "ColliderShape.h"

class ICollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	virtual void Initialize(ColliderShape shape);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

public:
	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク
	uint32_t collisionMask_ = 0xffffffff;

public:
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
	// 形状のポインタ
	ColliderShape colliderShape_;
};
