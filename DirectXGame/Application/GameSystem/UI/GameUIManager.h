#pragma once

/// <summary>
/// オブジェクトに依存しないUI管理クラス
/// </summary>
class GameUIManager
{
public:
	void Initialize();
	void Draw();

	// 操作説明系
	void HelpUIInitialize();
	void HelpUIDraw();

	//std::vector<>

};

