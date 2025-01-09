#pragma once
#include "Application/GameObject/Bullet/Trail/BulletTrail.h"
#include "Application/GameObject/Bullet/BulletEnums.h"
#include <vector>
#include <memory>
#include <bitset>

class ICamera;
class GPUParticleManager;

/// <summary>
/// 軌跡のインスタンス管理クラス
/// </summary>
class TrailManager
{
public:
	TrailManager();
	~TrailManager() = default;
	// パーティクルのシステム設定
	void SetGPUParticle(GPUParticleManager* gpuParticle) { gpuParticle_ = gpuParticle; }

public:
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(ICamera* camera);
	/// <summary>
	/// インスタンスの追加
	/// </summary>
	/// <param name="trail"></param>
	void AddTrail(std::unique_ptr<BulletTrail> trail);
	/// <summary>
	/// インスタンス削除
	/// </summary>
	void ClearTrail() { trails_.clear(); }

	BulletTrail* FindTrail(const std::string& name);

public:
	void SetFlags(const std::bitset<size_t(TrackingAttribute::kMaxSize)>& flags) {
		isInvisibles_ = flags;
	}
	size_t GetContainairSize() { return trails_.size(); }
private:
	// 軌跡インスタンスのコンテナ
	std::vector<std::unique_ptr<BulletTrail>> trails_;

	GPUParticleManager* gpuParticle_ = nullptr;

	// 描画フラグ
	std::bitset<size_t(TrackingAttribute::kMaxSize)> isInvisibles_;

};
