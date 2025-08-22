#pragma once
#include "Engine/3D/WorldTransform.h"
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

public:
	/// <summary>
	/// 属性取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetAttribute() const { return collisionAttribute_; }
	/// <summary>
	/// マスク取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetMask() const { return collisionMask_; }
	/// <summary>
	/// 属性設定
	/// </summary>
	/// <param name="attribute"></param>
	void SetAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	/// <summary>
	/// マスク設定
	/// </summary>
	/// <param name="mask"></param>
	void SetMask(uint32_t mask) { collisionMask_ = mask; }
	ColliderObject GetColliderObject() const { return objectRegistry_; }

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
