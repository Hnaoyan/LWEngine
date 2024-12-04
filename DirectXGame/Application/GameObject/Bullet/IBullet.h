#pragma once
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Trail/BulletTrail.h"
#include "User/TrackingState/StateMachine/BulletStateMachine.h"

/// <summary>
/// 弾の基底クラス
/// </summary>
class IBullet : public InstancedUnit
{
public:
	static uint32_t sSerialNumber;
	uint32_t serialNumber_ = 0u;

	~IBullet() override {
		trail_ = nullptr;
		object_ = nullptr;
	}

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
	Vector3 accelerate_ = {};
	// 軌跡
	BulletTrail* trail_ = nullptr;
	// 対象オブジェクト
	IGameObject* object_ = nullptr;

	// ダメージ量
	float damageRatio_ = 1.0f;

public: // アクセッサ
	std::string GetTag() const { return tag_; }
	EulerTransform GetTransform() const { return transform_; }
	Vector3 GetGeneratePosition() const { return generatePosition_; }
	Vector3 GetVelocity() const { return velocity_; }
	Vector3 GetAccelerate() const { return accelerate_; }

	Sphere* GetCollider() { return &collider_; }
	IGameObject* GetTarget() { return object_; }
	float DamageRatio() const { return damageRatio_; }

	void SetDamageRatio(const float& ratio) { damageRatio_ = ratio; }
	void SetTrail(BulletTrail* trail) { trail_ = trail; }
	void SetBreakEmitter(GPUParticleEmitter* emitter) { breakEmitter_ = emitter; }
	void SetGameObject(IGameObject* object) { object_ = object; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	void SetAccelerate(const Vector3& accelerate) { accelerate_ = accelerate; }

private:
	// 壊れるときのパーティクルのエミッター（今は使えない
	GPUParticleEmitter* breakEmitter_ = nullptr;

};