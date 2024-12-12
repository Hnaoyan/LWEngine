#pragma once
#include "../IGameObject.h"
#include "System/PlayerSystemLists.h"
#include "Module/Foot/PlayerFootCollider.h"
#include "Module/State/IPlayerState.h"
#include "System/PlayerFacade.h"

class SampleBulletManager;

/// <summary>
/// プレイヤー
/// </summary>
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
	/// <summary>
	/// コライダーの設定関数
	/// </summary>
	/// <param name="collisionManager"></param>
	void SetCollier(CollisionManager* collisionManager) override;

private:
	// システムファサードクラス
	std::unique_ptr<PlayerFacade> facadeSystem_;
	// ステート
	std::unique_ptr<PlayerStateManager> stateManager_;
	// マテリアル
	std::unique_ptr<Material> material_;
	// 操作関係
	OparationManager oparationManager_;
	// 足場コライダー
	PlayerFootCollider footCollider_;
	// AABBコライダー
	AABB collider_;

	// 読み込み用パス
	char path[256];
	std::string filePath;

	// 無敵時間のフレーム
	float energyRecover_ = 10.0f;

public:
	// 無敵時間
	float invisibleFrame_ = 30.0f;
	// 追従キャンセルの距離
	float trackCancelDistance = 75.0f;

public: // アクセッサ
	OparationManager* GetOperation() { return &oparationManager_; }
	PlayerFacade* GetSystemFacede() { return facadeSystem_.get(); }
	PlayerStateMachine* HorizontalState() { return stateManager_->GetHorizontal(); }
	PlayerStateMachine* VerticalState() { return stateManager_->GetVertical(); }
	Boss* GetBoss() { return boss_; }
	Material* GetMaterial() { return material_.get(); }
	// コライダー
	AABB* GetCollider() { return &collider_; }
	AABB* GetFootCollider() { return footCollider_.GetCollider(); }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Vector3 GetVelocity() const { return velocity_; }
	void SetColor(const Vector4& color) { material_->color_ = color; }
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

private: // USER
	// グローバル変数関係の初期化
	void InitializeGlobalValue() override;
	// 衝突の座標修正
	void CollisionCorrect(ICollider::CollisionType3D type, const Vector3& min, const Vector3& max);
	// 現在のステート
	void NowState();

public:
	// 移動速度
	Vector3 velocity_ = {};
	// カメラ
	ICamera* camera_ = nullptr;
	// 落下フラグ
	bool isGround_ = false;
	bool isDoubleJump_ = false;

private:
	// ボス
	Boss* boss_ = nullptr;

	// アウトライン用のモデル
	WorldTransform outlineTransform_{};
	Model* outlineModel_ = nullptr;
	std::unique_ptr<Material> outlineMaterial_;
};