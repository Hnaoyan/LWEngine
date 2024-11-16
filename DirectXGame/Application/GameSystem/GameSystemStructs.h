#pragma once

#include "Engine/Math/MathLib.h"

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
	PlayerKey<bool> keyConfigs;
	PlayerKey<int32_t> keybinds;
	Vector2 leftStick;
	Vector2 rightStick;
};

// リプレイ用の保存データ
struct ReplayData
{
	int32_t frameNumber = 0u;
	PlayerKey<bool> keyConfigs;
	Vector2 leftStick;
	Vector2 rightStick;
};

/// <summary>
/// ゲーム内のシステム的陣営
/// </summary>
enum class FactionType
{
	kPlayer,	// プレイヤー
	kEnemy,		// 敵
	kNeutral,	// 中立
};
