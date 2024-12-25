#include "GameUIManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/2D/TextureManager.h"
#include "Application/GameSystem/GameObjectManager.h"
#include "Application/GameSystem/GameSystem.h"

GameUIManager::GameUIManager(GameSystem* gameSystem)
{
	InitializeGlobalValue();
	// 操作関係のHUD
	AddHUD("Dash", TextureManager::Load("Resources/UI/DashUI.png"));
	AddHUD("Jump", TextureManager::Load("Resources/UI/JumpUItt.png"));
	AddHUD("LockOn", TextureManager::Load("Resources/UI/LockonUIt.png"));
	AddHUD("Shot", TextureManager::Load("Resources/UI/ShotUIt.png"));

	// それぞれの初期化
	gameClear_.sprite = SpriteManager::GetSprite("GameClearText");
	gameClear_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "ClearTextPos");

	gameOver_.sprite = SpriteManager::GetSprite("GameOverText");
	gameOver_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "OverTextPos");

	titleBack_.sprite = SpriteManager::GetSprite("ResultTitleUI");
	titleBack_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "TitleExceptUIPos");

	replay_.sprite = SpriteManager::GetSprite("ResultReplayUI");
	replay_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "ReplayExceptUIPos");

	cameraChange_.sprite = SpriteManager::GetSprite("CameraChangeUI");
	cameraChange_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "CameraChangeUIPos");

	gameStart_.sprite = SpriteManager::GetSprite("GameStartText");
	gameStart_.position = Vector2(1280.0f / 2.0f, 720.0f / 2.0f);

	gameSystem_ = gameSystem;
}

void GameUIManager::Initialize()
{

}

void GameUIManager::Update(GameSceneState state)
{
	sceneState_ = state;
}

void GameUIManager::Draw(GameObjectManager* gameObjectManager)
{
#ifdef IMGUI_ENABLED
	gameClear_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "ClearTextPos");
	gameOver_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "OverTextPos");
	titleBack_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "TitleExceptUIPos");
	replay_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "ReplayExceptUIPos");
	cameraChange_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "CameraChangeUIPos");
#endif // IMGUI_ENABLED

	// オブジェクトのUI
	gameObjectManager->UIDraw();

	// リプレイ中なら非表示に
	if (gameSystem_->IsReplayMode()) {
		titleBack_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "NowReplayTitleExceptUIPos");
		titleBack_.sprite->SetPosition(titleBack_.position);
		titleBack_.sprite->Draw();

		cameraChange_.sprite->SetPosition(cameraChange_.position);
		cameraChange_.sprite->Draw();
		return;
	}
	// ゲームオーバー・クリアの描画
	if (gameObjectManager->IsUIGameClear()) {
		gameClear_.sprite->SetPosition(gameClear_.position);
		gameClear_.sprite->Draw();
	}
	if (gameObjectManager->IsUIGameOver()) {
		gameOver_.sprite->SetPosition(gameOver_.position);
		gameOver_.sprite->Draw();
	}

	if (gameObjectManager->IsChange()) {
		titleBack_.sprite->SetPosition(titleBack_.position);
		titleBack_.sprite->Draw();
		replay_.sprite->SetPosition(replay_.position);
		replay_.sprite->Draw();
	}
	
	if (sceneState_ == GameSceneState::kGameTutorial) {
		gameStart_.sprite->SetPosition(gameStart_.position);
		gameStart_.sprite->Draw();
	}

#ifdef _DEBUG
	titleBack_.position = GlobalVariables::GetInstance()->GetValue<Vector2>("HUD", "NowReplayTitleExceptUIPos");
	titleBack_.sprite->SetPosition(titleBack_.position);
	titleBack_.sprite->Draw();
	cameraChange_.sprite->SetPosition(cameraChange_.position);
	cameraChange_.sprite->Draw();
#endif // _DEBUG

	// HUD関係
	for (std::vector<GameUI::UIData>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it) 
	{
		(*it).sprite->SetPosition((*it).position);
		(*it).sprite->Draw();
	}
}

void GameUIManager::AddHUD(std::string name, uint32_t texture)
{
	GameUI::UIData data = {};
	data.tag = name;
	data.texture = texture;
	data.sprite = SpriteManager::LoadSprite(name, texture);
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
	globalVariable->AddValue(groupName, "ComboTimerColor", Vector4(0.3f, 1.0f, 0.3f, 0.95f));	// 色
	globalVariable->AddValue(groupName, "ComboTimerSize", Vector2(24.0f, 280.0f));	// サイズ

	groupName = "HUD";

	globalVariable->AddValue(groupName, "DashPos", Vector2(139.0f, 60.0f));
	globalVariable->AddValue(groupName, "JumpPos", Vector2(139.0f, 180.0f));
	globalVariable->AddValue(groupName, "LockOnPos", Vector2(139.0f, 240.0f));
	globalVariable->AddValue(groupName, "ShotPos", Vector2(139.0f, 120.0f));

	globalVariable->AddValue(groupName, "ClearTextPos", Vector2(1280.0f / 2.0f, 720.0f / 2.0f));
	globalVariable->AddValue(groupName, "OverTextPos", Vector2(1280.0f / 2.0f, 720.0f / 2.0f));

	globalVariable->AddValue(groupName, "TitleExceptUIPos", Vector2(1280.0f / 2.0f, 720.0f / 2.0f));
	globalVariable->AddValue(groupName, "ReplayExceptUIPos", Vector2(1280.0f / 2.0f, 720.0f / 2.0f));
	
	globalVariable->AddValue(groupName, "NowReplayTitleExceptUIPos", Vector2(1280.0f / 2.0f, 720.0f / 2.0f));
	globalVariable->AddValue(groupName, "CameraChangeUIPos", Vector2(1280.0f / 2.0f + 200.0f, 720.0f));
}
