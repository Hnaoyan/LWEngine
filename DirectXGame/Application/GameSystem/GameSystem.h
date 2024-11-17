#pragma once
#include <stdint.h>
#include "GameSystemStructs.h"
#include "KeyConfigManager.h"
#include "Replay/ReplayManager.h"
#include "Engine/Utility/Singleton.h"

class Input;
/// <summary>
/// ゲームのシステム関係クラス
/// </summary>
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

	void ImGuiDraw();

	void LaunchReplay();

	//void
	CBufferDataBloom bloomData_{};
	CBufferDataVignette vignetteData_{};

private:
	// キー管理クラス
	KeyConfigManager keyConfig_;
	// リプレイ管理クラス
	ReplayManager replayManager_;
	// フレーム数のカウント
	uint32_t nowFrame_ = 0;
	// インプット
	Input* input_ = nullptr;

};
