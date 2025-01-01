#include "PlayerRoundShadow.h"
#include "Application/GameObject/Player/Player.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/2D/TextureManager.h"

#include <imgui.h>
#include <cassert>

void PlayerRoundShadow::Initialize(Player* player)
{
	assert(player);

	player_ = player;
	model_ = ModelManager::GetModel("RoundShadow");
	
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();
	material_->color_ = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	
	texture_ = TextureManager::Load("Resources/Effect/effect.png");

	worldTransform_.Initialize();
	//worldTransform_.parent_ = &player_->worldTransform_;
	worldTransform_.transform_.translate.y = -3.0f;
	worldTransform_.transform_.rotate.x = 1.57f;
	worldTransform_.UpdateMatrix();

	isInvisible_ = false;
}

void PlayerRoundShadow::Update()
{
	material_->Update();
	// プレイヤーの情報を適応
	Vector2 scale = Vector2(worldTransform_.transform_.scale.x, worldTransform_.transform_.scale.y);
	float t = LwLib::Normalize(player_->worldTransform_.GetWorldPosition().y, -1.0f, 26.0f);
	scale = Ease::Easing(Vector2(3.0f, 3.0f), Vector2(8.0f, 8.0f), t);

	worldTransform_.transform_.scale = Vector3(scale.x, scale.y, 1.0f);
	worldTransform_.transform_.translate.x = player_->worldTransform_.GetWorldPosition().x;
	worldTransform_.transform_.translate.z = player_->worldTransform_.GetWorldPosition().z;
	worldTransform_.UpdateMatrix();
}

void PlayerRoundShadow::Draw(ModelDrawDesc desc)
{
	// 早期リターン
	if (isInvisible_) {
		return;
	}
	// マテリアル更新
	model_->GetMaterial()->Update();
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();
	modelDesc.texture = texture_;
	// 本体の描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
}

void PlayerRoundShadow::ImGuiDraw()
{
	ImGui::SeparatorText("RoundShadow");
	ImGui::DragFloat3("T", &worldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("R", &worldTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("S", &worldTransform_.transform_.scale.x, 0.1f);
	ImGui::ColorEdit4("TColor", &material_->color_.x);
	ImGui::Checkbox("Invisible", &isInvisible_);
}
