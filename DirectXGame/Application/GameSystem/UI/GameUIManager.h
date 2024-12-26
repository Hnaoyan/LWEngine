#pragma once
#include "UIStructs.h"
#include "../GameSystemStructs.h"
#include <vector>

class GameObjectManager;
class GameSystem;

/// <summary>
/// オブジェクトに依存しないUI・UIのグローバル変数管理クラス
/// </summary>
class GameUIManager
{
public:
	GameUIManager(GameSystem* gameSystem);
	void Initialize();
	void Update(GameSceneState state);
	void Draw(GameObjectManager* gameObjectManager);
public:	// アクセッサ
	void SetIsHudHide(bool isHide) { isHUDHide_ = isHide; }
	bool IsHUDHide() { return isHUDHide_; }
private:// SYSTEM
	// HUDの追加
	void AddHUD(std::string name, uint32_t texture);
	// ゲームシステム
	GameSystem* gameSystem_ = nullptr;
	// HUDを消しておくかのフラグ
	bool isHUDHide_ = false;

	GameSceneState sceneState_ = GameSceneState::kWait;

private: // UI
	// HUD
	std::vector<GameUI::UIData> hudElements_;
	// クリアテキスト
	GameUI::TextUI gameClear_ = {};
	// オーバーテキスト
	GameUI::TextUI gameOver_ = {};
	// タイトルに戻るテキスト
	GameUI::TextUI titleBack_ = {};
	// リプレイテキスト
	GameUI::TextUI replay_ = {};
	// カメラ変更テキスト
	GameUI::TextUI cameraChange_ = {};

	// ゲームの開始テキスト
	GameUI::TextUI gameStart_ = {};

	struct TextComp
	{
		FrameTimer upper;
		FrameTimer lower;
		bool isAlpha;	// true:0→1 false:1→0
		GameUI::TextUI text;
	};

	TextComp gameStart = {};

private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();
};

