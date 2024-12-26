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

	~IBullet() override {
		trail_ = nullptr;
		targetObject_ = nullptr;
	}
private:
	// シリアル番号
	uint32_t serialNumber_ = 0u;
	// ダメージ量
	float damageRatio_ = 0.0f;

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
	// タグ
	std::string tag_;
	// 速度
	Vector3 velocity_ = {};
	// 加速度ベクトル
	Vector3 accelerate_ = {};
	// 軌跡
	BulletTrail* trail_ = nullptr;
	// 対象オブジェクト
	IGameObject* targetObject_ = nullptr;


public: // アクセッサ
	std::string GetTag() const { return tag_; }
	EulerTransform GetTransform() const { return transform_; }
	Vector3 GetVelocity() const { return velocity_; }
	Vector3 GetAccelerate() const { return accelerate_; }

	Sphere* GetCollider() { return &collider_; }
	IGameObject* GetTarget() { return targetObject_; }
	float DamageRatio() const { return damageRatio_; }

	void SetDamageRatio(const float& ratio) { damageRatio_ = ratio; }
	void SetTrail(BulletTrail* trail) { trail_ = trail; }
	void SetBreakEmitter(GPUParticleEmitter* emitter) { breakEmitter_ = emitter; }
	void SetGameObject(IGameObject* object) { targetObject_ = object; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	void SetAccelerate(const Vector3& accelerate) { accelerate_ = accelerate; }
public:
	/// <summary>
	/// ターゲットとの距離
	/// </summary>
	/// <returns></returns>
	float TargetToDistance();

private:
	// 壊れるときのパーティクルのエミッター（今は使えない
	GPUParticleEmitter* breakEmitter_ = nullptr;

};