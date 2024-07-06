#pragma once
#include "../IGameObject.h"
#include "System/PlayerSystemLists.h"

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

	Sphere* GetCollider() { return &collider_; }

public: // アクセッサ

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	void SetFollowCamera(ICamera* camera) { camera_ = camera; }

	void SetBulletManager(SampleBulletManager* manager) { systemManager_.SetManager(manager); }

	Vector3 GetAimReticle() { return aimManager_.GetWorldPosition(); }

private:
	/// <summary>
	/// システム関係の初期化
	/// </summary>
	void SystemInitialize();
	/// <summary>
	/// システム関係の更新
	/// </summary>
	void SystemUpdate();
	
public:
	// 移動速度
	Vector3 velocity_ = {};
	// カメラ
	ICamera* camera_ = nullptr;

private:
	// 操作関係
	OparationManager systemManager_;
	// エイム関係
	AimManager aimManager_;

	// AABBコライダー
	Sphere collider_;

};