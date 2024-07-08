#include "TerrainCluster.h"
#include "Terrain.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/Collision/CollisionManager.h"

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
	// 描画
	model_->InstancedDraw(desc, this->unitNum_, srvHandles_.second, texture_);

}

void TerrainCluster::TerrainRegister(const EulerTransform& transform)
{
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<Terrain>();
	instance->Initialize();
	instance->transform_ = transform;
	// リストにムーブ
	units_.push_back(std::move(instance));
}

void TerrainCluster::CollisionUpdate(CollisionManager* manager)
{
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {
		Terrain* obj = static_cast<Terrain*>((*it).get());
		manager->ListRegist(obj->GetCollider());
	}

}
