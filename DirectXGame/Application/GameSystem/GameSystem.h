#pragma once
#include <stdint.h>
#include "Engine/PostEffect/PostEffectRender.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"

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

	struct DashBlur
	{
		// ブラーの詳細情報
		CBufferDataBlur data;
		bool isActive = false;
		float maxWidth = 0.03f;
		FrameTimer timer;
		void Initialize() {
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
			data.blurWidth = maxWidth;
			timer.Start(35.0f);
			isActive = true;
		}
		void Finalize() {
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kAlpha;
			isActive = false;
		}
	};

public:
	// プレイヤーのキー
	static PlayerKeyConfig sPlayerKey;
	static DashBlur sBlurEffect;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//void
	CBufferDataBloom bloomData_{};
	CBufferDataVignette vignetteData_;

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
