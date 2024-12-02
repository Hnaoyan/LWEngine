#pragma once
#include "UIStructs.h"
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
	void Draw(GameObjectManager* gameObjectManager);

	void SetIsHudHide(bool isHide) { isHUDHide_ = isHide; }
	bool IsHUDHide() { return isHUDHide_; }
private:
	// HUD
	std::vector<GameUI::UIData> hudElements_;

	void AddHUD(std::string name, uint32_t texture);

	GameUI::TextUI gameClear_ = {};
	GameUI::TextUI gameOver_ = {};

	GameUI::TextUI titleBack_ = {};
	GameUI::TextUI replay_ = {};

	GameSystem* gameSystem_ = nullptr;

	// HUDを消しておくかのフラグ
	bool isHUDHide_ = false;

private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();
};

