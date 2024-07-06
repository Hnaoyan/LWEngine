#pragma once
#include "../../3D/WorldTransform.h"
#include "ColliderShape.h"
#include "Application/Collision/ColliderObject.h"

class ICollider
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
	virtual void Update() = 0;

public:
	/// <summary>
	/// 属性取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetAttribute() { return collisionAttribute_; }
	/// <summary>
	/// マスク取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetMask() { return collisionMask_; }
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
	/// <summary>
	/// クラスの取得
	/// </summary>
	/// <returns></returns>
	ColliderObject GetColliderObject() { return objectRegistry_; }
protected:
	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク
	uint32_t collisionMask_ = 0xffffffff;
	// 自分のオブジェクトタイプ
	ColliderObject objectRegistry_;

public:
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
};
