#pragma once
#include <stdint.h>
#include "Engine/PostEffect/PostEffectRender.h"

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

		void Initialize() {
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
		}
		void Finalize() {
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kAlpha;
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
	CBufferDataHSV bloomData_{};

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
