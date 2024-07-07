#include "TerrainCluster.h"

void TerrainCluster::Initialize(Model* model)
{
	// 基底クラス初期化
	InstancedGroup::Initialize(model);

}

void TerrainCluster::Update()
{
	// 基底クラス更新
	InstancedGroup::Update();
}

void TerrainCluster::Draw(ModelDrawDesc desc)
{
	// 基底クラス描画
	InstancedGroup::Draw(desc);
}
