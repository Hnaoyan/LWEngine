#include "GameUIManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

GameUIManager::GameUIManager()
{
	InitializeGlobalValue();
}

void GameUIManager::Initialize()
{
	HelpUIInitialize();
}

void GameUIManager::Draw()
{
	HelpUIDraw();
}

void GameUIManager::HelpUIInitialize()
{

}

void GameUIManager::HelpUIDraw()
{

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

}
