#include "GameUIManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/2D/SpriteManager.h"

GameUIManager::GameUIManager()
{
	InitializeGlobalValue();
}

void GameUIManager::Initialize()
{

}

void GameUIManager::Draw()
{

}

void GameUIManager::AddHUD(std::string name, uint32_t texture)
{
	GameUI::UIData data = {};
	data.tag = name;
	data.texture = texture;
	data.sprite = SpriteManager::GetSprite(name);
	data.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", name + "Pos");
	hudElements_.push_back(data);
}

void GameUIManager::InitializeGlobalValue()
{
	GlobalVariables* globalVariable = GlobalVariables::GetInstance();
	//---プレイヤー---//
	std::string groupName = "PlayerUI";
	globalVariable->CreateGroup(groupName);

	globalVariable->AddValue(groupName, "BGColor", Vector4(0.0f, 0.0f, 0.0f, 0.9f));	// 色

	// PlayerのHP
	globalVariable->AddValue(groupName, "HPPosition", Vector2(250.0f, 600.0f));	// 座標
	globalVariable->AddValue(groupName, "HPColor", Vector4(0.3f, 1.0f, 0.3f, 0.95f));	// 色
	globalVariable->AddValue(groupName, "HPSize", Vector2(380.0f, 48.0f));	// サイズ
	globalVariable->AddValue(groupName, "HPSizeRatio", Vector2(0.95f, 0.75f));	// サイズのレート（Size*Ratio)
	// エネルギー
	globalVariable->AddValue(groupName, "EnergyPosition", Vector2(1050.0f, 220.0f));	// 座標
	globalVariable->AddValue(groupName, "EnergyColor", Vector4(0.3f, 1.0f, 0.3f, 0.95f));	// 色
	globalVariable->AddValue(groupName, "EnergySize", Vector2(24.0f, 280.0f));	// サイズ
	globalVariable->AddValue(groupName, "EnergySizeRatio", Vector2(0.75f, 0.95f));	// サイズのレート
	//globalVariable->AddValue(groupName, "")

	groupName = "HUD";

	globalVariable->AddValue(groupName, "DashPos", Vector2(139.0f, 60.0f));
	globalVariable->AddValue(groupName, "JumpPos", Vector2(139.0f, 180.0f));
	globalVariable->AddValue(groupName, "LockOnPos", Vector2(139.0f, 240.0f));
	globalVariable->AddValue(groupName, "ShotPos", Vector2(139.0f, 120.0f));

}
