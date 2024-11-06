#pragma once
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Trail/BulletTrail.h"
#include "User/TrackingState/StateMachine/BulletStateMachine.h"

class IBullet : public InstancedUnit
{
public:
	static uint32_t sSerialNumber;
	uint32_t serialNumber_ = 0u;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	virtual void ImGuiDraw();
	/// <summary>
	/// コールバック関数
	/// </summary>
	/// <param name="object"></param>
	virtual void OnCollision(ColliderObject object);

protected:
	// コライダー
	Sphere collider_;
	// 速度
	Vector3 velocity_ = {};
	// 生成時の座標
	Vector3 generatePosition_{};
	// タグ
	std::string tag_;
	// 加速度ベクトル
	Vector3 accelerate_;

	BulletTrail* trail_ = nullptr;

	ParticleEmitter* breakEmitter_ = nullptr;

	// ステートマシン
	std::unique_ptr<BulletStateMachine> stateMachine_;
	// 対象オブジェクト
	IGameObject* object_ = nullptr;

public: // アクセッサ
	std::string GetTag() { return tag_; }
	EulerTransform GetTransform() { return transform_; }
	Sphere* GetCollider() { return &collider_; }
	Vector3 GetGeneratePosition() { return generatePosition_; }


	void SetTrail(BulletTrail* trail) { trail_ = trail; }
	void SetBreakEmitter(ParticleEmitter* emitter) { breakEmitter_ = emitter; }

	Vector3 GetVelocity() { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	Vector3 GetAccelerate() { return accelerate_; }
	void SetAccelerate(const Vector3& accelerate) { accelerate_ = accelerate; }
	IGameObject* GetTarget() { return object_; }
	void SetGameObject(IGameObject* object) { object_ = object; }

	BulletStateMachine* GetStateMachine() { return stateMachine_.get(); }
};