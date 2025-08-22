#pragma once
#include "Engine/Object/IGameObject2D.h"

class CollisionManager;

class Obstacle : public IGameObject2D
{
public:
	// シリアル番号
	static int sSerialNumber;
	// 名前
	std::string name_ = "";
public:
	void Initialize(Model* model) override;
	void Update() override;
	void ImGuiDraw() override;
	void OnCollision([[maybe_unused]] ColliderObject target) override;
	Rectangle2D* GetCollider();

	void SetCollision(CollisionManager* manager);
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
private:

	Vector3 size_ = {};

};

