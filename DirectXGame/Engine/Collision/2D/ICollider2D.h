#pragma once
#include "Engine/3D/WorldTransform.h"
#include "../ColliderShape.h"
#include "Engine/3D/WorldTransform.h"
#include "Application/Collision/ColliderObject.h"

class ICollider2D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	virtual void Initialize(ColliderObject object);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(const Vector2& worldPosition);

protected:
	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク
	uint32_t collisionMask_ = 0xffffffff;
	// 自分のオブジェクトタイプ
	ColliderObject objectRegistry_;
	// トランスフォーム
	WorldTransform worldTransform_;
};
