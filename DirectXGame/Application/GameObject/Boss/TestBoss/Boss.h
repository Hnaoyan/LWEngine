#pragma once
#include "../../IGameObject.h"
#include "../StatePattern/StateMachine.h"
#include "../System/BossSystem.h"
#include "../System/UI/BossUI.h"
#include "../System/BossFacade.h"
#include "../Animation/BossAnimationManager.h"

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
	/// <summary>
	/// UI描画
	/// </summary>
	void UIDraw();

	void Reset() { systemManager_->uiManager_.Initialize(this); }

private:
	void GlobalValueInitialize();

public:
	// ステート関係
	BossState::IState* GetState() { return state_.get(); }
	BossState::IState* GetPrevState() { return prevState_.get(); }
	BossState::StateVariant GetNowState() { return nowVariantState_; }
	BossState::StateManager* StateManager() { return &stateManager_; }
	BossState::StateDecider* GetDecider() { return &stateDecider_; }
	BossAnimationManager* GetAnimManager() { return animationManager_.get(); }
	BossSystemContext::HealthManager* GetHealth() { return &systemManager_->healthManager_; }
	// 弾発射までの間隔
	FrameTimer fireTimer_;
	BossSystemContext::BulletManager* GetBulletManager() { return bulletManager_.get(); }
	BossSystemContext::ParticleManager* GetParticleManager() { return &systemManager_->particleManager_; }

	// 外部
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
	// システムの窓口
	std::unique_ptr<BossFacade> systemManager_;
	// アニメーション用のやつ
	std::unique_ptr<BossAnimationManager> animationManager_;

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
