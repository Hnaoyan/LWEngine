#include "GroundCluster.h"

void GroundCluster::Initialize(Model* model)
{
	// 基底クラス初期化
	InstancedGroup::Initialize(model);

}

void GroundCluster::Update()
{
	// 基底クラス更新
	InstancedGroup::Update();
}

void GroundCluster::Draw(ModelDrawDesc desc)
{
	// 基底クラス描画
	InstancedGroup::Draw(desc);
}
