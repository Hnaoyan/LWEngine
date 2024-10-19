#pragma once
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Trail/BulletTrail.h"

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
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void Draw(ICamera* camera);

protected:
	// コライダー
	Sphere collider_;
	// 速度
	Vector3 velocity_ = {};
	// 生存時間（ダメージに変更を加える場合
	float lifeTime_ = 0.0f;
	// 生成時の座標
	Vector3 generatePosition_{};
	// タグ
	std::string tag_;
	// 加速度
	float acceleration_ = 0.0f;

public: // アクセッサ
	std::string GetTag() { return tag_; }
	EulerTransform GetTransform() { return transform_; }
	Sphere* GetCollider() { return &collider_; }
	Vector3 GetGeneratePosition() { return generatePosition_; }
	Vector3 GetVelocity() { return velocity_; }


	float GetLifeTime() { return lifeTime_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

};