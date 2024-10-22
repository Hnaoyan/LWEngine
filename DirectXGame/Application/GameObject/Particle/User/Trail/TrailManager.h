#pragma once
#include "Application/GameObject/Bullet/Trail/BulletTrail.h"
#include <vector>
#include <memory>

class ICamera;
class GPUParticleSystem;

/// <summary>
/// 軌跡のインスタンス管理クラス
/// </summary>
class TrailManager
{
public:
	TrailManager();
	~TrailManager() = default;
	// パーティクルのシステム設定
	void SetGPUParticle(GPUParticleSystem* gpuParticle) { gpuParticle_ = gpuParticle; }

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

private:
	// 軌跡インスタンスのコンテナ
	std::vector<std::unique_ptr<BulletTrail>> trails_;

	GPUParticleSystem* gpuParticle_ = nullptr;

};
