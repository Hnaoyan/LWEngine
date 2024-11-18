#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/PostEffect/PostEffectRender.h"

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
	PlayerKey<bool> keyConfigs;	// キーの状態
	PlayerKey<int32_t> keybinds;	// キーの配置
	Vector2 leftStick;	// 左スティック
	Vector2 rightStick;	// 右スティック
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

// ダッシュのブラーエフェクトの値
struct DashBlur
{
	// ブラーの詳細情報
	CBufferDataBlur data{};
	bool isActive = false;
	float maxWidth = 0.2f;
	FrameTimer timer;
	// 初期化
	void Initialize() {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
		data.blurWidth = maxWidth;
		timer.Start(35.0f);
		isActive = true;
	}
	// 終了処理
	void Finalize() {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
		isActive = false;
	}
};
