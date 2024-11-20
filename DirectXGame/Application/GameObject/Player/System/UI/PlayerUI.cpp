#include "PlayerUI.h"
#include "imgui.h"
#include "../../Player.h"
#include "Engine/2D/SpriteManager.h"

void PlayerContext::PlayerUIManager::Initialize(Player* player)
{
	player_ = player;

	reticle_ = SpriteManager::GetSprite("CrossHair");

	// HP系
	hpUI_.backGround.Initialize("PlayerHPBackUI");
	hpUI_.backGround.color = { 0.0f,0.0f,0.0f,0.9f };
	hpUI_.position = { 250.0f,600.0f };
	Vector2 scale = { 380.0f,48.0f };
	Vector2 scaleOffsetRatio = { 0.95f,0.75f };
	hpUI_.backGround.sprite->SetSize(scale);
	hpUI_.backGround.sprite->SetPosition({ hpUI_.position.x - (scale.x / 2.0f),hpUI_.position.y });
	hpUI_.backGround.sprite->SetAnchorPoint({ 0,0 });
	hpUI_.currentScale = scale * scaleOffsetRatio;
	hpUI_.currentHP.Initialize("PlayerGage");
	hpUI_.currentHP.color = { 0.3f,1.0f,0.3f,0.95f };
	hpUI_.currentHP.sprite->SetSize(hpUI_.currentScale);
	hpUI_.currentHP.sprite->SetPosition({ hpUI_.position.x - (scale.x / 2.0f) + ((scale.x - hpUI_.currentScale.x) / 2.0f),hpUI_.position.y + ((scale.y - hpUI_.currentScale.y) / 2.0f) });
	hpUI_.currentHP.sprite->SetAnchorPoint({ 0,0 });
	hpUI_.maxScale = hpUI_.currentHP.sprite->GetSize();
	//hpUI_.position = hpUI_.currentHP.sprite->GetPosition();

	// Energy系
	energyUI_.backGround.Initialize("PlayerEnergyBackUI");
	energyUI_.backGround.color = { 0.0f,0.0f,0.0f,0.9f };
	energyUI_.position = { 1050.0f,720.0f / 2.0f - (280.0f / 2.0f) };
	scale = { 24.0f,280.0f };
	scaleOffsetRatio = { 0.75f,0.95f };
	energyUI_.backGround.sprite->SetSize(scale);
	energyUI_.backGround.sprite->SetPosition({ energyUI_.position.x - (scale.x / 2.0f),energyUI_.position.y });
	energyUI_.backGround.sprite->SetAnchorPoint({ 0,0 });
	energyUI_.currentScale = scale * scaleOffsetRatio;
	energyUI_.currentHP.Initialize("PlayerEnergyGage");
	energyUI_.currentHP.color = { 0.3f,1.0f,0.3f,0.95f };
	energyUI_.currentHP.sprite->SetSize(energyUI_.currentScale);
	energyUI_.currentHP.sprite->SetPosition({ energyUI_.position.x - (scale.x / 2.0f) + ((scale.x - energyUI_.currentScale.x) / 2.0f),energyUI_.position.y + ((scale.y - energyUI_.currentScale.y) / 2.0f) });
	energyUI_.currentHP.sprite->SetAnchorPoint({ 0,0 });
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
	Vector2 scale = { 380.0f,48.0f };
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
	Vector2 scale = { 24.0f,280.0f };
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
