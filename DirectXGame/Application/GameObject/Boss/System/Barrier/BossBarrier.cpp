#include "BossBarrier.h"
#include "Application/GameObject/GameObjectLists.h"
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
	worldTransform_.transform_.scale = boss_->worldTransform_.transform_.scale;
	worldTransform_.transform_.scale *= 2.0f;

	material_ = std::make_unique<Material>();
	material_->CreateMaterial();
	material_->color_.w = 0.75f;
	// バリアの生成
	Create(20);
}

void BossSystemContext::BarrierManager::Update()
{
	if (param.isActive) {
		if (param.currentHP == 0) {
			param.isActive = false;
		}
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

void BossSystemContext::BarrierManager::Create(uint32_t generateValue)
{
	param.isActive = true;
	param.currentHP = generateValue;
	param.maxHP = generateValue;
}
