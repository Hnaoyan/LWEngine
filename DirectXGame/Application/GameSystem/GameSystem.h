#pragma once
#include <stdint.h>

class Input;

class GameSystem
{
public:
	static float sSpeedFactor;

	static float GameSpeedFactor();

private:
	template<typename T>
	struct PlayerKey
	{
		T lockon;
		T jump;
		T shot;
		T dash;
	};

	struct PlayerKeyConfig
	{
		PlayerKey<bool> keyConfigs_;
		PlayerKey<int32_t> keybinds_;
	};

public:
	static PlayerKeyConfig sPlayerKey;

	void Initialize();
	void Update();

private:
	void KeyBindUpdate();
	void KeyConfigUpdate();

	Input* input_ = nullptr;
};
