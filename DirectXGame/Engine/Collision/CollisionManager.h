#pragma once
#include "../Utility/Singleton.h"
#include "Collider/ICollider.h"
#include "Collider/ColliderShape.h"

#include <list>

class CollisionManager : public Singleton<CollisionManager>
{
public:
	// コライダーのリスト
	std::list<ColliderShape> colliders_;

public:
	/// <summary>
	/// 衝突処理
	/// </summary>
	void CheckAllCollisions();
	/// <summary>
	/// リストクリア
	/// </summary>
	void ListClear();
	/// <summary>
	/// リスト登録
	/// </summary>
	/// <param name="collider"></param>
	void ListRegist(ColliderShape collider);
	/// <summary>
	/// 衝突チェック
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(ColliderShape colliderA, ColliderShape colliderB);
};
