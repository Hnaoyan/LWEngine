#pragma once
#include "../../IGameObject.h"
#include "../StatePattern/StateMachine.h"
#include "../System/BossSystem.h"

class Player;

class Boss : public IGameObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc) override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	void ImGuiDraw() override;
	/// <summary>
	/// コールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderObject target) override;

	void Dead() { isDead_ = true; }

public:
	BossState::IState* GetState() { return state_.get(); }
	BossState::StateManager* StateManager() { return &stateManager_; }
	BossSystemContext::BulletManager* GetBulletManager() { return bulletManager_.get(); }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Sphere* GetCollider() { return &collider_; }

	// 弾発射までの間隔
	FrameTimer fireTimer_;

	Player* GetPlayer() { return player_; }
	void SetPlayer(Player* player) { player_ = player; }

	Vector3 respawnPos_ = {};

private:
	// コライダー
	Sphere collider_;
	// ステート管理
	BossState::StateManager stateManager_;
	// HPの管理
	BossSystemContext::HealthManager healthManager_;
	// 弾の管理
	std::unique_ptr<BossSystemContext::BulletManager> bulletManager_;
	// ステート
	std::unique_ptr<BossState::IState> state_;
	// プレイヤー
	Player* player_ = nullptr;
public:
	void ChangeState(std::unique_ptr<BossState::IState> newState) {
		state_ = std::move(newState);
	}


};
