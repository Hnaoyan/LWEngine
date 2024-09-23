#include "BossAnimationManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/3D/ModelRenderer.h"
#include "Engine/LwLib/Ease/Ease.h"

void BossAnimationManager::Initialize(Boss* boss)
{
	// 設定
	boss_ = boss;

	CreateHierarchy("Head", "BossEnemy", {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,1.5f,0.0f}});
	CreateHierarchy("Bottom", "BossEnemy", {{1.0f,1.0f,1.0f},{3.14f,0.0f,0.0f},{0.0f,-1.5f,0.0f}});

	AnimationExecute(AnimType::kClose);
}

void BossAnimationManager::Update()
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

		(*it).second.worldTransform.UpdateMatrix();
	}
}

void BossAnimationManager::Draw(ICamera* camera, DrawDesc::LightDesc lightDesc)
{
	DrawDesc::ModelDesc modelDesc{};
	for (std::unordered_map<std::string, Hierarchy>::iterator it = hierarchys_.begin(); it != hierarchys_.end(); ++it) {
		modelDesc.SetDesc((*it).second.model);
		modelDesc.worldTransform = &(*it).second.worldTransform;
		ModelRenderer::NormalDraw(camera, modelDesc, lightDesc);
	}
}

void BossAnimationManager::AnimationExecute(AnimType type)
{

	switch (type)
	{
	case BossAnimationManager::AnimType::kOpen:
		hierarchys_["Head"].worldTransform.transform_.translate.y = 1.5f;
		hierarchys_["Bottom"].worldTransform.transform_.translate.y = -1.5f;
		break;
	case BossAnimationManager::AnimType::kClose:
		hierarchys_["Head"].worldTransform.transform_.translate.y = 1.0f;
		hierarchys_["Bottom"].worldTransform.transform_.translate.y = -1.0f;
		break;
	default:
		break;
	}

}

void BossAnimationManager::AnimationExecute(AnimType type, float easeFrame)
{
	switch (type)
	{
	case BossAnimationManager::AnimType::kOpen:
		hierarchys_["Head"].easePoint = { Vector3(0.0f,1.0f,0.0f),Vector3(0.0f,1.5f,0.0f) };
		hierarchys_["Bottom"].easePoint = { Vector3(0.0f,-1.0f,0.0f),Vector3(0.0f,-1.5f,0.0f) };
		break;
	case BossAnimationManager::AnimType::kClose:
		hierarchys_["Head"].easePoint = { Vector3(0.0f,1.5f,0.0f),Vector3(0.0f,1.0f,0.0f) };
		hierarchys_["Bottom"].easePoint = { Vector3(0.0f,-1.5f,0.0f),Vector3(0.0f,-1.0f,0.0f) };
		break;
	default:
		break;
	}

	animTimer_.Start(easeFrame);
	animState_ = AnimState::kRun;
}

void BossAnimationManager::CreateHierarchy(std::string hierarchyName, std::string modelTag, const EulerTransform& transform)
{
	Hierarchy instance{};
	instance.model = ModelManager::GetModel(modelTag);
	instance.worldTransform.Initialize();
	instance.worldTransform.parent_ = &boss_->worldTransform_;
	instance.worldTransform.transform_ = transform;
	hierarchys_.emplace(hierarchyName, instance);
}
