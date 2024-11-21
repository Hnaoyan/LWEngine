#pragma once
#include "UIStructs.h"
#include <vector>

/// <summary>
/// オブジェクトに依存しないUI・UIのグローバル変数管理クラス
/// </summary>
class GameUIManager
{
public:
	GameUIManager();

	void Initialize();
	void Draw();

private:
	// HUD
	std::vector<GameUI::UIData> hudElements_;

	void AddHUD(std::string name, uint32_t texture);

private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();
};

