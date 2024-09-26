#include "BossAnimationManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/3D/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/DeltaTime.h"
#include "Engine/LwLib/Ease/Ease.h"

void BossSystemContext::AnimationManager::Initialize(Boss* boss)
{
	// 設定
	boss_ = boss;
	Vector3 scale = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "NormalScale");
	CreateHierarchy("Head", "BossEnemy", { scale,{0.0f,0.0f,0.0f},{0.0f,scale.x,0.0f}});
	CreateHierarchy("Bottom", "BossEnemy", { scale,{3.14f,0.0f,0.0f},{0.0f,-scale.x,0.0f}});
	CreateHierarchy("Core", "BarrierSphere", { scale / 1.5f,{},{} }, TextureManager::Load("Resources/Dissolve/noise0.png"));
	AnimationExecute(AnimType::kClose);
}

void BossSystemContext::AnimationManager::Update()
{
	// アニメーション用のタイマー
	animTimer_.Update();
	if (animTimer_.IsEnd()) {
		animState_ = AnimState::kIdle;
	}

	for (std::unordered_map<std::string, Hierarchy>::iterator it = hierarchys_.begin(); it != hierarchys_.end(); ++it) {
		// アニメーション中なら座標間の移動（仮置き
		if (animState_ == AnimState::kRun) {
			(*it).second.worldTransform.transform_.translate = Ease::Easing((*it).second.easePoint.first, (*it).second.easePoint.second, animTimer_.GetElapsedFrame());
		}
		if ((*it).first == "Core" && IsOpen()) {
			(*it).second.worldTransform.transform_.rotate.y += 1.0f * kDeltaTime;
		}
		(*it).second.worldTransform.UpdateMatrix();
	}
}

void BossSystemContext::AnimationManager::Draw(ICamera* camera, DrawDesc::LightDesc lightDesc)
{
	DrawDesc::ModelDesc modelDesc{};
	for (std::unordered_map<std::string, Hierarchy>::iterator it = hierarchys_.begin(); it != hierarchys_.end(); ++it) {
		modelDesc.SetDesc((*it).second.model);
		modelDesc.worldTransform = &(*it).second.worldTransform;
		modelDesc.texture = (*it).second.texture;
		ModelRenderer::NormalDraw(camera, modelDesc, lightDesc);
	}
}

void BossSystemContext::AnimationManager::AnimationExecute(AnimType type)
{
	Vector3 scale = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "NormalScale");

	switch (type)
	{
	case BossSystemContext::AnimationManager::AnimType::kOpen:
		hierarchys_["Head"].worldTransform.transform_.translate.y = (scale.x * 1.5f);
		hierarchys_["Bottom"].worldTransform.transform_.translate.y = -(scale.x * 1.5f);
		break;
	case BossSystemContext::AnimationManager::AnimType::kClose:
		hierarchys_["Head"].worldTransform.transform_.translate.y = scale.x;
		hierarchys_["Bottom"].worldTransform.transform_.translate.y = -scale.x;
		break;
	default:
		break;
	}
	animType_ = type;

}

void BossSystemContext::AnimationManager::AnimationExecute(AnimType type, float easeFrame)
{
	Vector3 scale = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "NormalScale");
	float BottomY = -scale.x;
	float HeadY = scale.x;
	switch (type)
	{
	case BossSystemContext::AnimationManager::AnimType::kOpen:
		hierarchys_["Head"].easePoint = { Vector3(0.0f,HeadY,0.0f),Vector3(0.0f,HeadY * 1.5f,0.0f) };
		hierarchys_["Bottom"].easePoint = { Vector3(0.0f,BottomY,0.0f),Vector3(0.0f, BottomY * 1.5f,0.0f) };
		break;
	case BossSystemContext::AnimationManager::AnimType::kClose:
		hierarchys_["Head"].easePoint = { Vector3(0.0f,HeadY * 1.5f,0.0f),Vector3(0.0f,HeadY,0.0f) };
		hierarchys_["Bottom"].easePoint = { Vector3(0.0f, BottomY * 1.5f,0.0f),Vector3(0.0f,BottomY,0.0f) };
		break;
	default:
		break;
	}

	animTimer_.Start(easeFrame);
	animState_ = AnimState::kRun;
	animType_ = type;
}

void BossSystemContext::AnimationManager::CreateHierarchy(std::string hierarchyName, std::string modelTag, const EulerTransform& transform)
{
	Hierarchy instance{};
	instance.model = ModelManager::GetModel(modelTag);
	instance.worldTransform.Initialize();
	instance.worldTransform.parent_ = &boss_->worldTransform_;
	instance.worldTransform.transform_ = transform;
	instance.texture = instance.model->GetModelData()->material.textureHandle;
	hierarchys_.emplace(hierarchyName, instance);
}

void BossSystemContext::AnimationManager::CreateHierarchy(std::string hierarchyName, std::string modelTag, const EulerTransform& transform, uint32_t texture)
{
	Hierarchy instance{};
	instance.model = ModelManager::GetModel(modelTag);
	instance.worldTransform.Initialize();
	instance.worldTransform.parent_ = &boss_->worldTransform_;
	instance.worldTransform.transform_ = transform;
	instance.texture = texture;
	hierarchys_.emplace(hierarchyName, instance);
}
