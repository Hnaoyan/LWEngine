#include "TerrainCluster.h"
#include "Engine/2D/TextureManager.h"

void TerrainCluster::Initialize(Model* model)
{
	// 基底クラス初期化
	InstancedGroup::Initialize(model);

	texture_ = TextureManager::Load("Resources/default/white2x2.png");

}

void TerrainCluster::Update()
{
	// 基底クラス更新
	InstancedGroup::Update();
}

void TerrainCluster::Draw(ModelDrawDesc desc)
{
	// 基底クラス描画
	//InstancedGroup::Draw(desc);

	model_->InstancedDraw(desc, this->unitNum_, srvHandles_.second, texture_);

}
