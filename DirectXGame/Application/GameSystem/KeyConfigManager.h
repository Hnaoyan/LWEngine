#pragma once
#include <stdint.h>
#include "Engine/Input/Input.h"

class KeyConfigManager
{
private:
	/// <summary>
	/// プレイヤーのキー
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	struct PlayerKey
	{
		T lockon;
		T jump;
		T shot;
		T quickBoost;
		T boost;
		T pressJump;
		T homingShot;
	};

	// キーコンフィグ
	struct PlayerKeyConfig
	{
		PlayerKey<bool> keyConfigs_;
		PlayerKey<int32_t> keybinds_;
	};
public:
	KeyConfigManager() {
		input_ = Input::GetInstance();
	}

public:

	PlayerKey<bool> GetPlayerKey() { return playerKey_.keyConfigs_; }

public:
	void Update();

private:
	// プレイヤーのキーコンフィグ
	PlayerKeyConfig playerKey_{};

	Input* input_ = nullptr;
};
