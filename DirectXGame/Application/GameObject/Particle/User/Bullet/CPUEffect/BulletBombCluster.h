#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Engine/Math/Vector/Vector3.h"
#include <memory>

class BulletBombEffect;

class BulletBombCluster : public InstancedGroup
{
public:
	BulletBombCluster() {};
	~BulletBombCluster() = default;

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

	void BulletBomb(const Vector3& position);

public:
	void AddParticle(std::unique_ptr<BulletBombEffect> particle);
	void AddParticle(const Vector3& position);

};