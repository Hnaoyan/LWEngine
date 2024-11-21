#pragma once
#include "../IGameObject.h"
#include "System/PlayerSystemLists.h"
#include "Module/Foot/PlayerFootCollider.h"
#include "Module/State/IPlayerState.h"
#include "System/PlayerFacade.h"

class SampleBulletManager;

class Player : public IGameObject
{
public:
	/// <summary>
	/// 初期化前処理
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="gpuParticle"></param>
	void PreInitialize(ICamera* camera, GPUParticleManager* gpuParticle);

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

#pragma region 内部システム関係
private:
	// システムファサードクラス
	std::unique_ptr<PlayerFacade> facadeSystem_;
	std::unique_ptr<Material> material_;
	// 操作関係
	OparationManager oparationManager_;
	// 足場コライダー
	PlayerFootCollider footCollider_;
	// AABBコライダー
	AABB collider_;

	// ステート
	std::unique_ptr<StateManager> horizontalState_;
	std::unique_ptr<StateManager> verticalState_;
	
	char path[256];
	std::string filePath;

public:
	OparationManager* GetOperation() { return &oparationManager_; }
	PlayerFacade* GetSystemFacede() { return facadeSystem_.get(); }
	StateManager* HorizontalManager() { return horizontalState_.get(); }
	StateManager* VerticalManager() { return verticalState_.get(); }
	Boss* GetBoss() { return boss_; }
#pragma endregion

public: // セッター
	// コライダー
	AABB* GetCollider() { return &collider_; }
	AABB* GetFootCollider() { return footCollider_.GetCollider(); }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Vector3 GetVelocity() { return velocity_; }
	// ポインタ関係
	void PointerInitialize(BulletManager* manager, Boss* boss, std::vector<std::unique_ptr<SampleEnemy>>* lists) {
		facadeSystem_->GetShootingManager()->SetManager(manager);
		boss_ = boss;
		oparationManager_.GetLockOn()->SetBoss(boss);
		oparationManager_.SetEnemyList(lists);
	}
	void SetBoss(Boss* boss) { 
		boss_ = boss;
		oparationManager_.GetLockOn()->SetBoss(boss);
	}

	void StateInitialize();

private: // USER
	// グローバル変数関係の初期化
	void InitializeGlobalValue() override;

	void CollisionCorrect(ICollider::CollisionType3D type, const Vector3& min, const Vector3& max);

public:
	// 移動速度
	Vector3 velocity_ = {};
	// カメラ
	ICamera* camera_ = nullptr;
	// 落下フラグ
	bool isGround_ = false;
private:
	// ボス
	Boss* boss_ = nullptr;
};