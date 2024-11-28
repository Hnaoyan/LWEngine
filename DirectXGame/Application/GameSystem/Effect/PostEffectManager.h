#pragma once
#include "../GameSystemStructs.h"

class PostEffectManager
{
public:
	PostEffectManager();
	void Update();
	void ImGuiDraw();
	void InitializeGlobalValue();
public:
	// 全体的なブルームの値
	CBufferDataBloom bloomData_{};
	// ダッシュのエフェクト
	static DashBlur sDashEffect;
	// ダメージのエフェクト
	static DamageVignette sDamageEffect;
};
