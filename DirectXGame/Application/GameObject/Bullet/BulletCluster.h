#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Builder/BulletBuilder.h"
#include "Factory/BulletFactory.h"
#include <memory>

/// <summary>
/// 前方宣言
/// </summary>
class GPUParticleSystem;
class CollisionManager;
class TrailManager;
class IBullet;
//class BulletBombCluster;

/// <summary>
/// 弾の集合体クラス
/// </summary>
class BulletCluster : public InstancedGroup 
{
public:
	static uint32_t sSerialNumber;
	uint32_t serialNumber_ = 0u;

public: // 仮想関数
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
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
public: // USER
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="manager"></param>
	void CollisionUpdate(CollisionManager* manager);
	/// <summary>
	/// 弾の追加
	/// </summary>
	/// <param name="position"></param>
	/// <param name="direct"></param>
	void AddBullet(std::unique_ptr<IBullet> bullet);
	void AddBullet(const BulletBuilder& builder, BulletType type);

	// 軌跡の管理ポインタ
	void SetTrailManager(TrailManager* trailManager) { trailManager_ = trailManager; }
	void SetGPUParticle(GPUParticleSystem* gpuParticle) { gpuParticle_ = gpuParticle; }
	void SetTexture(uint32_t texture) { texture_ = texture; }

	void SetColor(const Vector4& color) { material_->color_ = color; }
	void SetTrailColor(const Vector3& color) { trailColor_ = color; }
	//void SetElement();
	void SetName(const std::string& name) { name_ = name; }
private:
	// 弾が壊れた時のエフェクトクラス
	std::unique_ptr<InstancedGroup> bombEffectCluster_;

	std::unique_ptr<BulletFactory> bulletFactory_;

	// 
	std::string name_;

	uint32_t texture_ = 0;
	TrailManager* trailManager_ = nullptr;
	GPUParticleSystem* gpuParticle_ = nullptr;
	Vector3 trailColor_{1.0f,1.0f,1.0f};
};