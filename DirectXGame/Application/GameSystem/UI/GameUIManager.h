#pragma once
#include "UIStructs.h"

/// <summary>
/// オブジェクトに依存しないUI・UIのグローバル変数管理クラス
/// </summary>
class GameUIManager
{
public:
	GameUIManager();

	void Initialize();
	void Draw();

	// 操作説明系
	void HelpUIInitialize();
	void HelpUIDraw();
private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();
};

