#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/PostEffect/PostEffectRender.h"

/// <summary>
/// ゲームシーンの状態
/// </summary>
enum class GameSceneState
{
	kWait,
	kGameRestart,	// 再スタート
	kGameTutorial, // チュートリアル
	kGamePlay,	// スタート
	kReplay,	// リプレイ
	kRecord,	// 保存
};

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
	float maxWidth = 0.15f;
	FrameTimer timer;
	// 初期化
	void Initialize() {
		data.blurWidth = maxWidth;
		timer.Start(45.0f);
	}
	void Update() {
		timer.Update();
		data.blurWidth = Ease::Easing(maxWidth, 0.0f, timer.GetElapsedFrame());
	}
	// 終了処理
	void Finalize() {
		timer.End();
	}
};

// ダメージエフェクト
struct DamageVignette
{
	CBufferDataVignette data{};
	FrameTimer timer;
	float maxPow = 0.0f;
	// 初期化処理
	void Initialize() {
		float damageEffectFrame = 35.0f;
		timer.Start(damageEffectFrame);
		maxPow = GlobalVariables::GetInstance()->GetValue<float>("PostEffect", "VigPow");
	}
	// 更新処理
	void Update() {
		timer.Update();
		data.powValue = Ease::Easing(maxPow, 0.0f, timer.GetElapsedFrame());
	}
	// 終了処理
	void Finalize() {
		timer.End();
	}

};

struct GameStartVignette
{
	CBufferDataVignette data{};
	FrameTimer timer;
	float maxPow = 0.0f;
	float scale = 0.0f;
	// 初期化処理
	void Initialize() {
		float damageEffectFrame = 120.0f;
		maxPow = 0.4f;
		scale = 25.0f;
		timer.Start(damageEffectFrame);
	}
	// 更新処理
	void Update() {
		timer.Update();
		data.color = { 0.0f,0.0f,0.0f };
		data.powValue = Ease::Easing(maxPow, 0.0f, timer.GetElapsedFrame());
		data.scale = Ease::Easing(0.0f, scale, timer.GetElapsedFrame());
	}
	// 終了処理
	void Finalize() {
		timer.End();
	}
};

// ジャスト回避エフェクト
struct SlowGrayScale
{
	CBufferDataGrayscale data{};
	FrameTimer timer;

	void Initialize(float frame) {
		timer.Start(frame);
	}

	void Update() {
		timer.Update();
		data.lerpFactor = Ease::Easing(1.0f, 0.0f, timer.GetElapsedFrame());
	}

	void Finalize() {
		timer.End();
	}
};
