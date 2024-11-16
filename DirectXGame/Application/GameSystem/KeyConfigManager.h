#pragma once
#include <vector>
#include "GameSystemStructs.h"
#include "Engine/Input/Input.h"

class KeyConfigManager
{
public:
	KeyConfigManager();

	PlayerKey<bool> GetPlayerKey() { return playerKey_.keyConfigs; }
	PlayerKeyConfig* GetKeyConfig() { return &playerKey_; }
public:
	void Update();

private:
	// プレイヤーのキーコンフィグ
	PlayerKeyConfig playerKey_{};
	// リプレイモードか
	bool isReplay_ = false;
	// リプレイ用の保存コンテナ
	std::vector<ReplayData> replayContainer_;
	// 現在のフレーム

	Input* input_ = nullptr;
};
