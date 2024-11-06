#pragma once
#include <stdint.h>
#include "KeyConfigManager.h"
#include "Engine/Utility/Singleton.h"
#include "Engine/PostEffect/PostEffectRender.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"

class Input;

/// <summary>
/// ゲーム内のシステム的陣営
/// </summary>
enum class FactionType
{
	kPlayer,	// プレイヤー
	kEnemy,		// 敵
	kNeutral,	// 中立
};

class GameSystem : public Singleton<GameSystem>
{
public:
	// ゲームの速さ
	static float sSpeedFactor;
	inline static constexpr float sDefaultGameSpeed = 1.0f;
	/// <summary>
	/// ゲームの速さとデルタタイムでの値計算
	/// </summary>
	/// <returns></returns>
	static float GameSpeedFactor();

private: // ポストエフェクト用	
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
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
			isActive = false;
		}
	};

public:
	// プレイヤーのキー
	static DashBlur sBlurEffect;
	static KeyConfigManager sKeyConfigManager;

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
	CBufferDataVignette vignetteData_{};

private:
	KeyConfigManager keyConfig_;
	
	Input* input_ = nullptr;
};
