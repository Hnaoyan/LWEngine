#pragma once
#include "../IGameObject.h"
#include "System/PlayerSystemLists.h"
#include "Module/PlayerFootCollider.h"
#include "Module/State/IPlayerState.h"

class SampleBulletManager;

class Player : public IGameObject
{
public:
	/// <summary>
	/// 初期化前処理
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="gpuParticle"></param>
	void PreInitialize(ICamera* camera, GPUParticleSystem* gpuParticle) {
		camera_ = camera;
		particleManager_.SetGPUParticleSystem(gpuParticle);
	}

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
	void UISpriteDraw();

public: // ゲッター
	// コライダー
	AABB* GetCollider() { return &collider_; }
	AABB* GetFootCollider() { return footCollider_.GetCollider(); }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	OparationManager* GetOperation() { return &systemManager_; }
	StateManager* GetStateManager() { return &stateManager_; }
	PlayerContext::EnergyManager* GetEnergyManager() { return &energyManager_; }
	PlayerContext::HealthManager* GetHPManager() { return &healthManager_; }
	Vector3 GetVelocity() { return velocity_; }
public: // ステート
	IPlayerState* GetHorizontalState() { return currentStates_.first.get(); }
	IPlayerState* GetVerticalState() { return currentStates_.second.get(); }
	void SetHorizontalState(std::unique_ptr<IPlayerState> newState) { currentStates_.first = std::move(newState); }
	void SetVerticalState(std::unique_ptr<IPlayerState> newState) { currentStates_.second = std::move(newState); }

public: // セッター
	// ポインタ関係
	void PointerInitialize(BulletManager* manager, Boss* boss, std::vector<std::unique_ptr<SampleEnemy>>* lists) {
		systemManager_.GetLockOn()->SetBoss(boss); 
		systemManager_.SetManager(manager);
		systemManager_.SetEnemyList(lists);
	}
	void SetBoss(Boss* boss) { systemManager_.GetLockOn()->SetBoss(boss); }

private: // USER

	void CollisionCorrect(ICollider::CollisionType3D type, const Vector3& min, const Vector3& max);
	
public:
	// 前フレームの位置
	Vector3 prevPosition_ = {};
	// 移動速度
	Vector3 velocity_ = {};
	// カメラ
	ICamera* camera_ = nullptr;
	// 落下フラグ
	bool isGround_ = false;

private:
	// ステート
	std::pair<std::unique_ptr<IPlayerState>, std::unique_ptr<IPlayerState>> currentStates_;
	// 操作関係
	OparationManager systemManager_;
	// ステートのマネージャー
	StateManager stateManager_;
	// 足場コライダー
	PlayerFootCollider footCollider_;
	// AABBコライダー
	AABB collider_;
	// Hp関係
	PlayerContext::HealthManager healthManager_;
	// パーティクル
	PlayerContext::ParticleManager particleManager_;
	// エネルギー
	PlayerContext::EnergyManager energyManager_;
	// UI
	PlayerContext::UIManager uiManager_;
};