#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Factory/BulletFactory.h"
#include "Factory/Builder/BulletBuilder.h"
#include <memory>

/// <summary>
/// 前方宣言
/// </summary>
class GPUParticleManager;
class CollisionManager;
class TrailManager;
class IBullet;

class BulletBombCluster;

/// <summary>
/// 弾のクラスターインターフェース
/// </summary>
class IBulletCluster : public InstancedGroup
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

public:
	// 衝突の更新
	virtual void CollisionUpdate(CollisionManager* manager);
	// 弾の追加
	virtual void AddBullet(std::unique_ptr<IBullet> bullet);
	virtual void AddBullet(const BulletBuilder& builder, BulletType type);

	void SetBombCluster(BulletBombCluster* bombCluster) { bombCluster_ = bombCluster; }

protected:
	// 軌跡の色
	Vector3 trailColor_{ 1.0f,1.0f,1.0f };

protected: // 基底クラスに必要なやつ
	// 弾が壊れた時のエフェクトクラス
	std::unique_ptr<InstancedGroup> bombEffectCluster_;
	// ファクトリー
	std::unique_ptr<BulletFactory> bulletFactory_;
	// 爆発クラスター
	BulletBombCluster* bombCluster_ = nullptr;

	// マネージャのポインタ
	TrailManager* trailManager_ = nullptr;
	GPUParticleManager* gpuParticle_ = nullptr;

public: // アクセッサ系
	// 軌跡の管理ポインタ
	void SetTrailManager(TrailManager* trailManager) { trailManager_ = trailManager; }
	void SetGPUParticle(GPUParticleManager* gpuParticle) { gpuParticle_ = gpuParticle; }
	void SetColor(const Vector4& color) { material_->color_ = color; }
	void SetTrailColor(const Vector3& color) { trailColor_ = color; }

};
