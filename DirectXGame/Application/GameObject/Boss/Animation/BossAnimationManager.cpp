#include "BossAnimationManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/DeltaTime.h"
#include "Engine/LwLib/Ease/Ease.h"
#include <imgui.h>

void BossSystemContext::AnimationManager::Initialize(Boss* boss)
{
	// 設定
	boss_ = boss;
	Vector3 scale = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "NormalScale");
	// 部位作成処理
	CreateHierarchy("Head", "BossEnemy", { scale,{0.0f,0.0f,0.0f},{0.0f,scale.x,0.0f}});
	CreateHierarchy("Bottom", "BossEnemy", { scale,{3.14f,0.0f,0.0f},{0.0f,-scale.x,0.0f}});
	CreateHierarchy("Core", "BarrierSphere", { scale / 1.5f,{},{} }, TextureManager::Load("Resources/Dissolve/noise0.png"));
	// 閉じる処理
	AnimationExecute(AnimType::kClose);

	commonMaterial_ = std::make_unique<Material>();
	commonMaterial_->CreateMaterial();
}

void BossSystemContext::AnimationManager::Update()
{
	commonMaterial_->Update();

	hitEffect_.Update();
	// エフェクト中は
	if (hitEffect_.timer.IsActive()) {
		commonMaterial_->color_.w = hitEffect_.alpha;
	}
	// アニメーション用のタイマー
	animTimer_.Update();
	deathTimer_.Update();
	if (animTimer_.IsEnd()) {
		animState_ = AnimState::kIdle;
	}
	if (!deathTimer_.IsActive() && boss_->IsDead()) {
		deathTimer_.Start(300.0f);
	}

	// ヒエラルキーの更新
	for (std::unordered_map<std::string, Hierarchy>::iterator it = hierarchys_.begin(); it != hierarchys_.end(); ++it) {
		// アニメーション中なら座標間の移動（仮置き
		if (animState_ == AnimState::kRun) {
			(*it).second.worldTransform.transform_.translate = Ease::Easing((*it).second.easePoint.first, (*it).second.easePoint.second, animTimer_.GetElapsedFrame());
		}
		if ((*it).first == "Core" && IsOpen()) {
			(*it).second.worldTransform.transform_.rotate.y += 1.0f * kDeltaTime;
		}

		if (boss_->IsDead()) {
			if ((*it).first == "Core") {
				(*it).second.worldTransform.transform_.scale = LwLib::LerpToCurrent<Vector3>((*it).second.worldTransform.transform_.scale, Vector3::Zero(), deathTimer_.GetElapsedFrame() * 2.0f);

			}
			else {
				(*it).second.worldTransform.transform_.scale = LwLib::LerpToCurrent<Vector3>((*it).second.worldTransform.transform_.scale, Vector3::Zero(), deathTimer_.GetElapsedFrame());
			}
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
		modelDesc.material = commonMaterial_.get();
		ModelRenderer::NormalDraw(camera, modelDesc, lightDesc);
	}
}

void BossSystemContext::AnimationManager::ImGuiDraw()
{
	ImGui::SeparatorText("Animation:Model");
	ImGui::ColorEdit4("MaterialColor", &commonMaterial_->color_.x);
	ImGui::DragFloat("LampFrame", &hitEffect_.returnFrame, 0.01f);
}

void BossSystemContext::AnimationManager::AnimationExecute(AnimType type)
{
	Vector3 scale = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "NormalScale");
	float openValue = 1.5f;

	switch (type)
	{
	case BossSystemContext::AnimationManager::AnimType::kOpen:
		hierarchys_["Head"].worldTransform.transform_.translate.y = (scale.x * openValue);
		hierarchys_["Bottom"].worldTransform.transform_.translate.y = -(scale.x * openValue);
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
	float openValue = 1.5f;

	switch (type)
	{
	case BossSystemContext::AnimationManager::AnimType::kOpen:
		hierarchys_["Head"].easePoint = { Vector3(0.0f,HeadY,0.0f),Vector3(0.0f,HeadY * openValue,0.0f) };
		hierarchys_["Bottom"].easePoint = { Vector3(0.0f,BottomY,0.0f),Vector3(0.0f, BottomY * openValue,0.0f) };
		break;
	case BossSystemContext::AnimationManager::AnimType::kClose:
		hierarchys_["Head"].easePoint = { Vector3(0.0f,HeadY * openValue,0.0f),Vector3(0.0f,HeadY,0.0f) };
		hierarchys_["Bottom"].easePoint = { Vector3(0.0f, BottomY * openValue,0.0f),Vector3(0.0f,BottomY,0.0f) };
		break;
	default:
		break;
	}

	// アニメーション関係
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
