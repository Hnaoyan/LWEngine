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

public:
	// ステート関係
	BossState::IState* GetState() { return state_.get(); }
	BossState::IState* GetPrevState() { return prevState_.get(); }
	BossState::StateVariant GetNowState() { return nowVariantState_; }
	BossState::StateManager* StateManager() { return &stateManager_; }
	BossState::StateDecider* GetDecider() { return &stateDecider_; }
	// 弾発射までの間隔
	FrameTimer fireTimer_;
	BossSystemContext::BulletManager* GetBulletManager() { return bulletManager_.get(); }
	BossSystemContext::ParticleManager* GetParticleManager() { return &particleManager_; }

	GPUParticleSystem* GetGPUParticle() { return gpuParticle_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Sphere* GetCollider() { return &collider_; }
	Player* GetPlayer() { return player_; }
	void SetPlayer(Player* player) { 
		player_ = player; 
		stateDecider_.Initialize(this, player);
		bulletManager_->SetPlayer(player);
	}
	void SetNowVariantState(BossState::StateVariant variant) { nowVariantState_ = variant; }
	void SetPrevVariantState(BossState::StateVariant variant) { prevVariantState_ = variant; }
	void SetGPUParticle(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }

	Vector3 respawnPos_ = {};
private: // サブシステム
	// ステート管理
	BossState::StateManager stateManager_;
	BossState::StateDecider stateDecider_;
	BossState::StateVariant nowVariantState_;
	BossState::StateVariant prevVariantState_;
	// HPの管理
	BossSystemContext::HealthManager healthManager_;
	BossSystemContext::ParticleManager particleManager_;

private:
	// コライダー
	Sphere collider_;

	// 弾の管理
	std::unique_ptr<BossSystemContext::BulletManager> bulletManager_;
	// ステート
	std::unique_ptr<BossState::IState> prevState_;
	std::unique_ptr<BossState::IState> state_;
	// プレイヤー
	Player* player_ = nullptr;
	GPUParticleSystem* gpuParticle_ = nullptr;

	FrameTimer curveTime_;
	bool isHalf_ = false;

public: // 内部でのみ呼び出す関数
	/// <summary>
	/// ステート変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<BossState::IState> newState) {
		if (state_) {
			prevState_ = std::move(state_);
		}
		state_ = std::move(newState);
	}
};
