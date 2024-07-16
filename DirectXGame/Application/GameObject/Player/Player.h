#pragma once
#include "../IGameObject.h"
#include "System/PlayerSystemLists.h"
#include "Module/PlayerFootCollider.h"
#include "Module/State/Common/IPlayerState.h"
#include "Module/State/StateMachine.h"

class SampleBulletManager;

class Player : public IGameObject
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
	void UISpriteDraw();

public: // アクセッサ
	// コライダー
	AABB* GetCollider() { return &collider_; }
	AABB* GetFootCollider() { return footCollider_.GetCollider(); }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	OparationManager* GetOperation() { return &systemManager_; }

	// ポインタ関係
	void SetFollowCamera(ICamera* camera) { camera_ = camera; }
	void SetBulletManager(SampleBulletManager* manager) { systemManager_.SetManager(manager); }
	void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { systemManager_.SetEnemyList(lists); }
	// ステート
	IPlayerState* GetState() { return currentState_.get(); }
	void SetState(std::unique_ptr<IPlayerState> newState) {
		currentState_ = std::move(newState);
	}
	// stateの取得
	StateMachine* GetVerticalState() { return VerticalState_.get(); }
	StateMachine* GetHorizontalState() { return HorizontalState_.get(); }
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
	std::unique_ptr<IPlayerState> currentState_;

	// それぞれのステート
	std::unique_ptr<StateMachine> VerticalState_;
	std::unique_ptr<StateMachine> HorizontalState_;

	// 操作関係
	OparationManager systemManager_;
	// 足場コライダー
	PlayerFootCollider footCollider_;
	// AABBコライダー
	AABB collider_;


};