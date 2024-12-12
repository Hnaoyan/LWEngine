#pragma once
#include "../../../../Collision/ColliderObject.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class Player;

/// <summary>
/// 足場コライダー
/// </summary>
class PlayerFootCollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 衝突
	/// </summary>
	/// <param name="target"></param>
	void OnCollision(ColliderObject target);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
public: // アクセッサ
	AABB* GetCollider() { return &collider_; }

private:
	Player* player_ = nullptr;
	WorldTransform worldTransform_;
	AABB collider_;
	Vector3 scale_ = {};
};