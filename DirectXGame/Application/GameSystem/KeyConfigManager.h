#pragma once
#include <stdint.h>

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

	PlayerKey<bool> GetPlayerKey() { return playerKey_.keyConfigs_; }

public:
	void Update();

private:
	// プレイヤーのキーコンフィグ
	PlayerKeyConfig playerKey_{};
};
