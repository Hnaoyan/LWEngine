#pragma once
#include <stdint.h>

class GameSystem
{
public:
	static float sSpeedFactor;

	static float GameSpeedFactor();

public:

	template<typename T>
	struct PlayerKey
	{
		T lockon;
		T jump;
		T shot;
		T dash;
	};
	
	static PlayerKey<bool> flags_;
	static PlayerKey<int32_t> keybinds_;

};
