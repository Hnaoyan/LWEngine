#include "PlayerUI.h"
#include "imgui.h"
#include "../../Player.h"
#include "Engine/2D/SpriteManager.h"

void PlayerContext::UIManager::Initialize(Player* player)
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
	this->backUI_ = hpUI_.backGround.sprite->GetPosition();

	hpUI_.currentScale = scale * scaleOffsetRatio;

	hpUI_.currentHP.Initialize("PlayerGage");
	hpUI_.currentHP.color = { 0.3f,1.0f,0.3f,0.95f };
	hpUI_.currentHP.sprite->SetSize(hpUI_.currentScale);
	
	hpUI_.currentHP.sprite->SetPosition({ hpUI_.position.x - (scale.x / 2.0f) + ((scale.x - hpUI_.currentScale.x) / 2.0f),hpUI_.position.y + ((scale.y - hpUI_.currentScale.y) / 2.0f) });
	hpUI_.currentHP.sprite->SetAnchorPoint({ 0,0 });

	hpUI_.maxScale = hpUI_.currentHP.sprite->GetSize();
	//hpUI_.position = hpUI_.currentHP.sprite->GetPosition();

}

void PlayerContext::UIManager::Draw()
{
	reticle_->SetPosition(player_->GetOperation()->GetAimManager()->GetReticlePosition());
	reticle_->Draw();

	HPUI();

}

void PlayerContext::UIManager::HPUI()
{
	Vector2 scale = { 380.0f,48.0f };
	// 現在値
	hpUI_.currentScale = Vector2::Lerp({ 0.0f,hpUI_.maxScale.y }, hpUI_.maxScale, player_->GetHPManager()->GetHPRatio());
	hpUI_.currentHP.sprite->SetSize(hpUI_.currentScale);
	hpUI_.currentHP.Draw();
	// 背景
	hpUI_.backGround.sprite->SetPosition({ hpUI_.position.x - (scale.x / 2.0f),hpUI_.position.y });
	hpUI_.backGround.Draw();

}

void PlayerContext::UIManager::ImGuiDraw()
{
	ImGui::DragFloat2("Position", &hpUI_.position.x, 0.01f);
	Vector2 scale = hpUI_.currentHP.sprite->GetSize();
	ImGui::DragFloat2("Scale", &scale.x, 1.0f);
	hpUI_.currentHP.sprite->SetSize(scale);
}
