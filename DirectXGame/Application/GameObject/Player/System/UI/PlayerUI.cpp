#include "PlayerUI.h"
#include "imgui.h"
#include "../../Player.h"
#include "Engine/2D/SpriteManager.h"

void PlayerContext::PlayerUIManager::Initialize(Player* player)
{
	player_ = player;

	reticle_ = SpriteManager::GetSprite("CrossHair");

	globalVariables_ = GlobalVariables::GetInstance();

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
}

void PlayerContext::PlayerUIManager::Draw()
{
	reticle_->SetPosition(player_->GetOperation()->GetAimManager()->GetReticlePosition());
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
	energyUI_.currentScale = Vector2::Lerp({ energyUI_.maxScale.x, 0.0f }, energyUI_.maxScale, t);
	energyUI_.currentHP.sprite->SetSize(energyUI_.currentScale);
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
}
