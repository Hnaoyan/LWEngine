#include "BossBarrier.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/Ease/Ease.h"

#include "imgui.h"
#include <cassert>

void BossSystemContext::BarrierManager::Initialize(Boss* boss)
{
	assert(boss);
	boss_ = boss;
	model_ = ModelManager::GetModel("BarrierSphere");
	textureHandle_ = TextureManager::GetInstance()->Load("Resources/default/GNPalse.png");

	worldTransform_.Initialize();
	worldTransform_.parent_ = &boss_->worldTransform_;

	material_ = std::make_unique<Material>();
	material_->CreateMaterial();
	material_->color_.w = 0.85f;
	// バリアの生成
	//Create(GlobalVariables::GetInstance()->GetValue<float>("Boss", "BarrierHP"));
}

void BossSystemContext::BarrierManager::Update()
{
	// クールタイムの処理
	recoveryTime_.Update();
	// 生成
	if (recoveryTime_.IsEnd()) {
		Create(GlobalVariables::GetInstance()->GetValue<float>("Boss", "BarrierHP"));
	}

	// アニメーションの処理
	// タイマー処理
	animParam.vanishTimer.Update();
	animParam.reappearTimer.Update();
	// 補間処理
	if (animParam.vanishTimer.IsActive()) {
		material_->threshold_ = Ease::Easing(0.0f, 1.0f, animParam.vanishTimer.GetElapsedFrame());
	}
	else if (animParam.reappearTimer.IsActive()) {
		material_->color_.w = 0.85f;
		material_->threshold_ = Ease::Easing(1.0f, 0.0f, animParam.reappearTimer.GetElapsedFrame());
	}

	// マテリアル
	material_->color_.w = std::clamp((param.currentHP + 1.0f) / param.maxHP, 0.35f, 0.85f);
	material_->Update();
	// モデル用の情報
	worldTransform_.UpdateMatrix();
}

void BossSystemContext::BarrierManager::Draw(ICamera* camera, const DrawDesc::LightDesc& lightDesc)
{
	if (material_->threshold_ != 1.0f) {
		// 設定
		DrawDesc::ModelDesc modelDesc{};
		modelDesc.SetDesc(model_);
		modelDesc.worldTransform = &worldTransform_;
		modelDesc.texture = textureHandle_;
		modelDesc.material = material_.get();
		// 描画
		ModelRenderer::NormalDraw(camera, modelDesc, lightDesc);
	}
}

void BossSystemContext::BarrierManager::ImGuiDraw()
{
	ImGui::ColorEdit3("DissolveColor", &material_->dissolveColor_.x);
	ImGui::DragFloat("Thresold", &material_->threshold_, 0.01f, 0.0f, 1.0f);
}

void BossSystemContext::BarrierManager::Create(float generateValue)
{
	// パラメータ
	param.isActive = true;
	param.currentHP = generateValue;
	param.maxHP = generateValue;
	// サイズの設定
	worldTransform_.transform_.scale = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "BarrierScale");
	worldTransform_.UpdateMatrix();
	// アニメーション
	animParam.reappearTimer.Start(30.0f);
}

void BossSystemContext::BarrierManager::DamageProcess(float damage)
{
	if (param.currentHP + damage >= 0.0f) {
		param.currentHP += damage;
	}
	// 割れる処理
	else {
		param.currentHP = 0.0f;
		param.isShattered = true;
		animParam.vanishTimer.Start(60.0f);
	}
}

void BossSystemContext::BarrierManager::BarrierBreak()
{
	param.isActive = false;
	param.isShattered = false;
	recoveryTime_.Start(600.0f);
}
