#pragma once
#include "../IGameObject.h"
#include "StatePattern/StateMachine.h"
#include "System/BossSystem.h"
#include "System/UI/BossUI.h"
#include "System/BossFacade.h"
#include "Animation/BossAnimationManager.h"
#include "System/BossSystemLists.h"

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
	void Finalize();

private:
	// グローバル変数関係の初期化
	void GlobalValueInitialize() override;
	// グローバル変数関係の更新
	//void GlobalValueUpdate();

	bool isAction_ = false;

private: // サブシステム
	// ステート管理
	BossState::StateManager stateManager_;
	BossState::StateDecider stateDecider_;
	// ステート
	std::unique_ptr<BossState::IState> prevState_;
	std::unique_ptr<BossState::IState> state_;
	BossState::StateVariant nowVariantState_;
	BossState::StateVariant prevVariantState_;
	// システムの窓口
	std::unique_ptr<BossFacade> systemManager_;
	// アニメーション用のやつ
	std::unique_ptr<BossSystemContext::AnimationManager> animationManager_;
	// 弾の管理
	std::unique_ptr<BossSystemContext::BulletManager> bulletManager_;

#pragma region 内部システムのアクセッサ
public:
	// ステート関係
	BossState::IState* GetState() { return state_.get(); }
	BossState::IState* GetPrevState() { return prevState_.get(); }
	BossState::StateVariant GetNowState() { return nowVariantState_; }
	BossState::StateManager* StateManager() { return &stateManager_; }
	BossState::StateDecider* GetDecider() { return &stateDecider_; }
	void SetNowVariantState(BossState::StateVariant variant) { nowVariantState_ = variant; }
	void SetPrevVariantState(BossState::StateVariant variant) { prevVariantState_ = variant; }

	// コンテキストのアクセッサ
	BossSystemContext::HealthManager* GetHealth() { return &systemManager_->healthManager_; }
	BossSystemContext::BulletManager* GetBulletManager() { return bulletManager_.get(); }
	BossSystemContext::ParticleManager* GetParticleManager() { return &systemManager_->particleManager_; }
	BossSystemContext::AnimationManager* GetAnimManager() { return animationManager_.get(); }

	Vector3 respawnPos_ = {};
#pragma endregion

#pragma region 外部関係のポインタやアクセッサ
private:
	// コライダー
	Sphere collider_;
	// プレイヤー
	Player* player_ = nullptr;
	// GPU
	GPUParticleSystem* gpuParticle_ = nullptr;
public:
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
	void SetGPUParticle(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }
#pragma endregion

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
