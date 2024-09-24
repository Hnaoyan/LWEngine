#include "BossBarrier.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/3D/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"
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
	material_->color_.w = 0.75f;
	// バリアの生成
	Create(15);
}

void BossSystemContext::BarrierManager::Update()
{
	// クールタイムの処理
	recoveryTime_.Update();
	// 生成
	if (recoveryTime_.IsEnd()) {
		Create(15);
	}
	// マテリアル
	material_->Update();
	// モデル用の情報
	worldTransform_.UpdateMatrix();
}

void BossSystemContext::BarrierManager::Draw(ICamera* camera, const DrawDesc::LightDesc& lightDesc)
{
	if (param.isActive) {
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

void BossSystemContext::BarrierManager::Create(float generateValue)
{
	// パラメータ
	param.isActive = true;
	param.currentHP = generateValue;
	param.maxHP = generateValue;
	// サイズの設定
	worldTransform_.transform_.scale = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "BarrierScale");
	worldTransform_.UpdateMatrix();
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
	}
}

void BossSystemContext::BarrierManager::BarrierBreak()
{
	param.isActive = false;
	param.isShattered = false;
	recoveryTime_.Start(600.0f);
}
