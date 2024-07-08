#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"

class CollisionManager;

class TerrainCluster : public InstancedGroup
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
	/// ブロックの追加
	/// </summary>
	/// <param name="transform"></param>
	void TerrainRegister(const EulerTransform& transform);

	/// <summary>
	/// 衝突登録
	/// </summary>
	/// <param name="manager"></param>
	void CollisionUpdate(CollisionManager* manager);

private:
	uint32_t texture_ = 0;
};
