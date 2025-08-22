#pragma once
#include "Engine/Math/MathLib.h"

class Player;

class PlayerController
{
public:
	void Initialize(Player* player);
	void Update();
	void MovementHandler();

private:
	Player* player_ = nullptr;

	// 速度関係
	Vector2 moveDirection_ = {};
	float speedRatio_ = 0.0f;
	Vector2 velocity_ = {};
};

