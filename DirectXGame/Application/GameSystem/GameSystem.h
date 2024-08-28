#pragma once
#include <stdint.h>

class Input;

class GameSystem
{
public:
	// ゲームの速さ
	static float sSpeedFactor;
	/// <summary>
	/// ゲームの速さとデルタタイムでの値計算
	/// </summary>
	/// <returns></returns>
	static float GameSpeedFactor();

private:
	template<typename T>
	struct PlayerKey
	{
		T lockon;
		T jump;
		T shot;
		T quickBoost;
		T boost;
		T pressJump;
	};

	struct PlayerKeyConfig
	{
		PlayerKey<bool> keyConfigs_;
		PlayerKey<int32_t> keybinds_;
	};

public:
	// プレイヤーのキー
	static PlayerKeyConfig sPlayerKey;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	/// <summary>
	/// キーバインドの更新
	/// </summary>
	void KeyBindUpdate();
	/// <summary>
	/// キーコンフィグの状態更新
	/// </summary>
	void KeyConfigUpdate();

	Input* input_ = nullptr;
};
