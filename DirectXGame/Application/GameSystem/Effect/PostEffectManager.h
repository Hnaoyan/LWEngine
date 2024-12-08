#pragma once
#include "../GameSystemStructs.h"

class GameSystem;

/// <summary>
/// ゲーム内でのポストエフェクト管理クラス
/// </summary>
class PostEffectManager
{
public:
	PostEffectManager();
	void Initialize(GameSystem* system);
	void Update();
	void ImGuiDraw();
	void InitializeGlobalValue();
public:
	// 現在のエフェクト
	Pipeline::PostEffectType nowEffect;
	// 全体的なブルームの値
	CBufferDataBloom bloomData_{};
	// ダッシュのエフェクト
	static DashBlur sDashEffect;
	// ダメージのエフェクト
	static DamageVignette sDamageEffect;
	// スローモーション処理
	static SlowGrayScale sSlowEffect;
private:
	GameSystem* gameSystem_ = nullptr;

};
