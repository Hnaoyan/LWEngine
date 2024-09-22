#include "BossAnimationManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/3D/ModelRenderer.h"

void BossAnimationManager::Initialize(Boss* boss)
{
	// 設定
	boss_ = boss;

	CreateHierarchy("BossEnemy", { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,1.5f,0.0f} });
	CreateHierarchy("BossEnemy", { {1.0f,1.0f,1.0f},{3.14f,0.0f,0.0f},{0.0f,-1.5f,0.0f} });

}

void BossAnimationManager::Update()
{
	for (std::vector<Hierarchy>::iterator it = hierarchyObj_.begin(); it != hierarchyObj_.end(); ++it) {
		(*it).worldTransform.UpdateMatrix();
	}
}

void BossAnimationManager::Draw(ICamera* camera, DrawDesc::LightDesc lightDesc)
{
	DrawDesc::ModelDesc modelDesc{};
	for (std::vector<Hierarchy>::iterator it = hierarchyObj_.begin(); it != hierarchyObj_.end(); ++it) {
		modelDesc.SetDesc((*it).model);
		modelDesc.worldTransform = &(*it).worldTransform;
		ModelRenderer::NormalDraw(camera, modelDesc, lightDesc);
	}
}

void BossAnimationManager::CreateHierarchy(std::string modelTag, const EulerTransform& transform)
{
	Hierarchy instance{};
	instance.model = ModelManager::GetModel(modelTag);
	instance.worldTransform.Initialize();
	instance.worldTransform.parent_ = &boss_->worldTransform_;
	instance.worldTransform.transform_ = transform;
	hierarchyObj_.push_back(instance);
}
