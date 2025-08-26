#pragma once
#include "Engine/Object/IGameObject2D.h"
#include "System/PlayerSystemLists.h"

class CollisionManager;
class GameStateManager;

class Player : public IGameObject2D
{
public:
	void Initialize(Model* model) override;
	void Update() override;
	void ImGuiDraw() override;
	void OnCollision([[maybe_unused]] ColliderObject target) override;

public: // アクセッサ
	void SetGameStateManager(GameStateManager* manager) { gameStateManager_ = manager; }

private:
	GameStateManager* gameStateManager_ = nullptr;
	// 操作関係
	std::unique_ptr<PlayerController> controller_;
	bool isGoal_ = false;
};
