#include "PlayerUI.h"
#include "imgui.h"
#include "../../Player.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

void PlayerContext::PlayerUIManager::Initialize(Player* player)
{
	assert(player);
	// 基底
	ISystem::Initialize(player);
	reticle_ = SpriteManager::GetSprite("CrossHair");
	// HP系
	std::string groupName = "PlayerUI";
	hpUI_.position = globalVariables_->GetValue<Vector2>(groupName, "HPPosition");
	Vector2 scale = globalVariables_->GetValue<Vector2>(groupName, "HPSize");
	Vector2 scaleOffsetRatio = globalVariables_->GetValue<Vector2>(groupName, "HPSizeRatio");
	hpUI_.currentScale = scale * scaleOffsetRatio;
	hpUI_.currentHP.Initialize("PlayerGage");
	hpUI_.currentHP.color = globalVariables_->GetValue<Vector4>(groupName, "HPColor");
	hpUI_.currentHP.sprite->SetSize(hpUI_.currentScale);
	hpUI_.currentHP.sprite->SetPosition({ hpUI_.position.x - (scale.x / 2.0f) + ((scale.x - hpUI_.currentScale.x) / 2.0f),hpUI_.position.y + ((scale.y - hpUI_.currentScale.y) / 2.0f) });
	hpUI_.currentHP.sprite->SetAnchorPoint({});
	
	hpUI_.backGround.Initialize("PlayerHPBackUI");
	hpUI_.backGround.color = globalVariables_->GetValue<Vector4>(groupName, "BGColor");
	hpUI_.backGround.sprite->SetSize(scale);
	hpUI_.backGround.sprite->SetPosition({ hpUI_.position.x - (scale.x / 2.0f),hpUI_.position.y });
	hpUI_.backGround.sprite->SetAnchorPoint({});

	hpUI_.maxScale = hpUI_.currentHP.sprite->GetSize();

	// Energy系
	energyUI_.position = globalVariables_->GetValue<Vector2>(groupName, "EnergyPosition");
	scale = globalVariables_->GetValue<Vector2>(groupName, "EnergySize");
	scaleOffsetRatio = globalVariables_->GetValue<Vector2>(groupName, "EnergySizeRatio");
	energyUI_.currentScale = scale * scaleOffsetRatio;
	energyUI_.currentHP.Initialize("PlayerEnergyGage");
	energyUI_.currentHP.color = globalVariables_->GetValue<Vector4>(groupName, "EnergyColor");
	energyUI_.currentHP.sprite->SetSize(energyUI_.currentScale);
	energyUI_.currentHP.sprite->SetPosition({ energyUI_.position.x - (scale.x / 2.0f) + ((scale.x - energyUI_.currentScale.x) / 2.0f),energyUI_.position.y + ((scale.y - energyUI_.currentScale.y) / 2.0f) });
	energyUI_.currentHP.sprite->SetAnchorPoint({});

	energyUI_.backGround.Initialize("PlayerEnergyBackUI");
	energyUI_.backGround.color = globalVariables_->GetValue<Vector4>(groupName, "BGColor");
	energyUI_.backGround.sprite->SetSize(scale);
	energyUI_.backGround.sprite->SetPosition({ energyUI_.position.x - (scale.x / 2.0f),energyUI_.position.y });
	energyUI_.backGround.sprite->SetAnchorPoint({});

	energyUI_.maxScale = energyUI_.currentHP.sprite->GetSize();

	// ジャスト回避コンボ
	justDodgeUI_.model = ModelManager::GetModel("Plane");
	justDodgeUI_.material = std::make_unique<Material>();
	justDodgeUI_.material->CreateMaterial();
	justDodgeUI_.material->color_ = { 1.0f,1.0f,1.0f,1.0f };
	justDodgeUI_.material->shininess_ = 100.0f;
	justDodgeUI_.texture = TextureManager::Load("Resources/default/white2x2.png");
	justDodgeUI_.worldTransform.Initialize();
	justDodgeUI_.offset = { 0.0f,5.0f,0.0f };
	justDodgeUI_.maxScale = { 2.0f,0.2f,1.0f };
}

void PlayerContext::PlayerUIManager::Update()
{
}

void PlayerContext::PlayerUIManager::Draw()
{
	reticle_->SetPosition(player_->GetWeaponManager()->GetAimManager()->GetReticlePosition());
	reticle_->Draw();

	HPUI();
	EnergyUI();
}

void PlayerContext::PlayerUIManager::HPUI()
{
	Vector2 scale = globalVariables_->GetValue<Vector2>("PlayerUI", "HPSize");
	// 現在値
	float t = player_->GetSystemFacede()->GetHealth()->GetHPRatio();
	hpUI_.currentScale = Vector2::Lerp({ 0.0f,hpUI_.maxScale.y }, hpUI_.maxScale,t);
	hpUI_.currentHP.sprite->SetSize(hpUI_.currentScale);
	hpUI_.currentHP.Draw();
	// 背景
	hpUI_.backGround.sprite->SetPosition({ hpUI_.position.x - (scale.x / 2.0f),hpUI_.position.y });
	hpUI_.backGround.Draw();

}

void PlayerContext::PlayerUIManager::EnergyUI()
{
	Vector2 scale = globalVariables_->GetValue<Vector2>("PlayerUI", "EnergySize");
	// 現在値
	float t = player_->GetSystemFacede()->GetEnergy()->GetEnergyRatio();
	Vector4 color = { 0.0f,1.0f,0.0f,1.0f };
	// オーバーヒートしているなら色を赤に
	if (player_->GetSystemFacede()->GetEnergy()->IsOverheat()) {
		Vector4 red = { 1.0f,0.0f,0.0f,1.0f };
		color = red;
	}
	energyUI_.currentScale = Vector2::Lerp({ energyUI_.maxScale.x, 0.0f }, energyUI_.maxScale, t);
	energyUI_.currentHP.sprite->SetSize(energyUI_.currentScale);
	energyUI_.currentHP.color = (color);
	energyUI_.currentHP.Draw();
	// 背景
	energyUI_.backGround.sprite->SetPosition({ energyUI_.position.x - (scale.x / 2.0f),energyUI_.position.y });
	energyUI_.backGround.Draw();
}

void PlayerContext::PlayerUIManager::ImGuiDraw()
{
	ImGui::DragFloat2("Position", &hpUI_.position.x, 0.01f);
	Vector2 scale = hpUI_.currentHP.sprite->GetSize();
	ImGui::DragFloat2("Scale", &scale.x, 1.0f);
	hpUI_.currentHP.sprite->SetSize(scale);
	ImGui::DragFloat3("ComboUIOffset", &justDodgeUI_.offset.x, 0.1f);
	ImGui::DragFloat3("JustComboScale", &justDodgeUI_.worldTransform.transform_.scale.x, 0.1f);
	ImGui::ColorEdit4("PlaneColor", &justDodgeUI_.material->color_.x);
	ImGui::DragFloat("PlaneShinnes", &justDodgeUI_.material->shininess_);
}

void PlayerContext::PlayerUIManager::ComboGageUpdate()
{
	


}

void PlayerContext::PlayerUIManager::Draw(ModelDrawDesc desc)
{
	// ジャスト回避用の
	if (player_->GetSystemFacede()->GetDudgeManager()->GetComboKeepTimer()->IsActive()) {
		justDodgeUI_.isInvisible_ = false;
		justDodgeUI_.worldTransform.transform_.scale = Ease::Easing(justDodgeUI_.maxScale, Vector3(0.0f, justDodgeUI_.maxScale.y, justDodgeUI_.maxScale.z), player_->GetSystemFacede()->GetDudgeManager()->GetComboKeepTimer()->GetElapsedFrame());
	}
	else {
		return;
	}
	// UIのやつ
	justDodgeUI_.worldTransform.transform_.translate = player_->worldTransform_.transform_.translate;
	justDodgeUI_.Update();
	justDodgeUI_.worldTransform.transform_.rotate = desc.camera->transform_.rotate;
	justDodgeUI_.worldTransform.UpdateMatrix();
	justDodgeUI_.material->Update();
	justDodgeUI_.isInvisible_ = true;

	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(justDodgeUI_.model);
	modelDesc.worldTransform = &justDodgeUI_.worldTransform;
	modelDesc.material = justDodgeUI_.material.get();
	modelDesc.texture = justDodgeUI_.texture;
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
}
