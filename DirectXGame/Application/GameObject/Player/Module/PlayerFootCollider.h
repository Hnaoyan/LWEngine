#pragma once
#include "../../../Collision/ColliderObject.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class Player;

class PlayerFootCollider
{
public:
	void Initialize(Player* player);
	void Update();
	void OnCollision(ColliderObject target);
	void ImGuiDraw();
public: // アクセッサ
	AABB* GetCollider() { return &collider_; }

private:
	Player* player_ = nullptr;
	WorldTransform worldTransform_;
	AABB collider_;
	Vector3 scale_ = {};
};