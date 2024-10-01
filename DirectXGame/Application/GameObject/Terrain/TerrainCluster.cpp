#include "TerrainCluster.h"
#include "Terrain.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelRenderer.h"
#include "Engine/Collision/CollisionManager.h"

#include "imgui.h"

uint32_t TerrainCluster::sSerialNumber = 0;

void TerrainCluster::Initialize(Model* model)
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 基底クラス初期化
	InstancedGroup::Initialize(model);

	if (name_ == "Object") {
		texture_ = TextureManager::Load("Resources/default/BackGround.png");
	}
	else {
		texture_ = TextureManager::Load("Resources/default/white2x2.png");
	}

}

void TerrainCluster::Update()
{
	// 基底クラス更新
	InstancedGroup::Update();
}

void TerrainCluster::Draw(ModelDrawDesc desc)
{
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	material_->Update();
	modelDesc.material = material_.get();
	modelDesc.texture = texture_;
	// 描画
	ModelRenderer::InstancedDraw(desc.camera, modelDesc, lightDesc, this->unitNum_, buffer_.GetSRVGPU());

}

void TerrainCluster::ImGuiDraw()
{
	std::string name;
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {
		Terrain* obj = static_cast<Terrain*>((*it).get());
		name = "Terrain" + std::to_string(obj->serialNumber_);
		if (ImGui::TreeNode(name.c_str())) {
			obj->ImGuiDraw();
			ImGui::TreePop();
		}
	}
}

void TerrainCluster::TerrainRegister(const EulerTransform& transform)
{
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<Terrain>();
	instance->transform_ = transform;
	static_cast<Terrain*>(instance.get())->Initialize();
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
