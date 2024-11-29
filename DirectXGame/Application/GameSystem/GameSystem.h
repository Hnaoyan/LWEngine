#pragma once
#include <stdint.h>
#include "GameSystemStructs.h"
#include "UI/GameUIManager.h"
#include "KeyConfig/KeyConfigManager.h"
#include "Replay/ReplayManager.h"
#include "Effect/PostEffectManager.h"
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
	static KeyConfigManager sKeyConfigManager;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// リプレイの開始処理
	/// </summary>
	void LaunchReplay();

public:
	ReplayManager* GetReplayManager() { return &replayManager_; }
private:
	// キー管理クラス
	KeyConfigManager keyConfig_;
	// リプレイ管理クラス
	ReplayManager replayManager_;
	// ポストエフェクト管理クラス
	PostEffectManager postEffectManager_;

	// フレーム数のカウント
	uint32_t recordNowFrame_ = 0;
	// インプット
	Input* input_ = nullptr;

};
