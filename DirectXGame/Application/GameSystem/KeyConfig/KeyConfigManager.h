#pragma once
#include <vector>
#include "../GameSystemStructs.h"
#include "Engine/Input/Input.h"

class ReplayManager;

/// <summary>
/// キーの処理管理クラス
/// </summary>
class KeyConfigManager
{
public:
	KeyConfigManager();

	PlayerKey<bool> GetPlayerKey() { return playerKey_.keyConfigs; }
	PlayerKeyConfig* GetKeyConfig() { return &playerKey_; }
public:
	void Update(ReplayManager* replayManager);

	void BeginReplay();

private:
	// プレイヤーのキーコンフィグ
	PlayerKeyConfig playerKey_{};
	// リプレイモードか
	bool isReplay_ = false;

	Input* input_ = nullptr;
};
