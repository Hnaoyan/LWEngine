#include "BossUI.h"
#include "imgui.h"

#include "Application/GameObject/Boss/TestBoss/Boss.h"

#include "Engine/2D/SpriteManager.h"
#include "Engine/2D/TextureManager.h"

void BossSystemContext::UIManager::Initialize(Boss* boss)
{
	// ポインタ
	boss_ = boss;

	// HP系
	hpUI_.backGround.Initialize("HPBackUI");
	hpUI_.backGround.color = { 0.0f,0.0f,0.0f,0.7f };

	Vector2 position = { 640.0f,64.0f };
	Vector2 scale = { 512.0f,26.0f };
	Vector2 scaleOffsetRatio = { 0.98f,0.75f };
	hpUI_.backGround.sprite->SetSize(scale);
	hpUI_.backGround.sprite->SetPosition({ position.x - (scale.x / 2.0f),position.y });
	hpUI_.backGround.sprite->SetAnchorPoint({ 0,0 });

	hpUI_.currentScale = scale * scaleOffsetRatio;
	hpUI_.maxScale = hpUI_.currentScale;

	hpUI_.currentHP.Initialize("Gage");
	hpUI_.currentHP.color = { 1.0f,1.0f,1.0f,0.95f };
	hpUI_.currentHP.sprite->SetSize(hpUI_.currentScale);
	position = { position.x - (scale.x / 2.0f) + ((scale.x - hpUI_.currentScale.x) / 2.0f),position.y + ((scale.y - hpUI_.currentScale.y) / 2.0f) };
	hpUI_.currentHP.sprite->SetPosition(position);
	hpUI_.currentHP.sprite->SetAnchorPoint({ 0,0 });
	hpUI_.position = hpUI_.currentHP.sprite->GetPosition();

	
}

void BossSystemContext::UIManager::Draw()
{
	// 現在値
	hpUI_.currentScale = Vector2::Lerp({ 0.0f,hpUI_.maxScale.y }, hpUI_.maxScale, boss_->GetHealth()->GetHPRatio());
	hpUI_.currentHP.sprite->SetSize(hpUI_.currentScale);
	hpUI_.currentHP.sprite->SetPosition(hpUI_.position);
	hpUI_.currentHP.Draw();
	// 背景
	hpUI_.backGround.Draw();
}

void BossSystemContext::UIManager::ImGuiDraw()
{
	ImGui::DragFloat2("Position", &hpUI_.position.x, 0.01f);
	ImGui::DragFloat2("Scale", &hpUI_.currentScale.x, 0.01f);
}
