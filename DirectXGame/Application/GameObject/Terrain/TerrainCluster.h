#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"

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
private:
	uint32_t texture_ = 0;
};
