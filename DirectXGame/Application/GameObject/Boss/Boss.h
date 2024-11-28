#pragma once
#include "../IGameObject.h"

#include "Application/GameObject/Bullet/BulletManager.h"
#include "StatePattern/StateMachine.h"
#include "StatePattern/BossStateDecider.h"
#include "StatePattern/Attack/BossMissileBarrage.h"
#include "Animation/BossAnimationManager.h"
#include "System/BossSystem.h"
#include "System/UI/BossUI.h"
#include "System/BossFacade.h"
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
	/// アニメーションの更新
	/// </summary>
	void AnimationUpdate();
	/// <summary>
	/// UI描画
	/// </summary>
	void UIDraw();
	/// <summary>
	/// リセット
	/// </summary>
	void Reset() { systemManager_->uiManager_.Initialize(this); }
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();
private:
	// グローバル変数関係の初期化
	void InitializeGlobalValue() override;
	// グローバル変数関係の更新
	//void UpdateGlobalValue()　override;

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

	// 外部の弾管理
	BulletManager* bulletManager_ = nullptr;

	ICamera* camera_ = nullptr;

#pragma region 内部システムアクセッサ
public:
	// ステート関係
	BossState::IState* GetState() { return state_.get(); }
	BossState::IState* GetPrevState() { return prevState_.get(); }
	BossState::StateManager* StateManager() { return &stateManager_; }
	BossState::StateDecider* GetDecider() { return &stateDecider_; }
	BossState::StateVariant GetNowState() const { return nowVariantState_; }

	BossFacade* GetSystem() { return systemManager_.get(); }
	void SetNowVariantState(BossState::StateVariant variant) { nowVariantState_ = variant; }
	void SetPrevVariantState(BossState::StateVariant variant) { prevVariantState_ = variant; }
	void SetCamera(ICamera* camera) { camera_ = camera; }
	// コンテキストのアクセッサ
	BossSystemContext::HealthManager* GetHealth() { return &systemManager_->healthManager_; }
	BossSystemContext::ParticleManager* GetParticleManager() { return &systemManager_->particleManager_; }
	BossSystemContext::AnimationManager* GetAnimManager() { return animationManager_.get(); }

	Vector3 HitEffectPosition();
#pragma endregion

#pragma region 外部関係のポインタやアクセッサ
private:
	// コライダー
	Sphere collider_;
	// プレイヤー
	Player* player_ = nullptr;
	// GPU
	GPUParticleManager* gpuParticle_ = nullptr;
public:
	void SetIsAction(bool isAction) { isAction_ = isAction; }
	// 外部
	GPUParticleManager* GetGPUParticle() { return gpuParticle_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Sphere* GetCollider() { return &collider_; }
	Player* GetPlayer() { return player_; }

	void SetPlayer(Player* player) {
		player_ = player;
		stateDecider_.Initialize(this, player);
	}
	void SetGPUParticle(GPUParticleManager* ptr) { gpuParticle_ = ptr; }
	void SetBulletManager(BulletManager* bulletManager) { bulletManager_ = bulletManager; }
	BulletManager* GetBulletManager() { return bulletManager_; }
	IBulletCluster* GetTrackingCluster() { return bulletManager_->FindCluster("Boss:TrackingBullet"); }
	IBulletCluster* GetSuperiorCluster() { return bulletManager_->FindCluster("Boss:Superior"); }
	IBulletCluster* GetInferiorCluster() { return bulletManager_->FindCluster("Boss:Inferior"); }
	IBulletCluster* GetGeneusCluster() { return bulletManager_->FindCluster("Boss:Genius"); }
	IBulletCluster* GetNormalBulletCluster() { return bulletManager_->FindCluster("Boss:NormalBullet"); }
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
