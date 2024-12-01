#pragma once
#include "UIStructs.h"
#include <vector>

class GameObjectManager;

/// <summary>
/// オブジェクトに依存しないUI・UIのグローバル変数管理クラス
/// </summary>
class GameUIManager
{
public:
	GameUIManager();
	void Initialize();
	void Draw(GameObjectManager* gameObjectManager);

private:
	// HUD
	std::vector<GameUI::UIData> hudElements_;

	void AddHUD(std::string name, uint32_t texture);

	GameUI::TextUI gameClear_ = {};
	GameUI::TextUI gameOver_ = {};

	GameUI::TextUI titleBack_ = {};
	GameUI::TextUI replay_ = {};

private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();
};

