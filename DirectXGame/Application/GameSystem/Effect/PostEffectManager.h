#pragma once
#include "../GameSystemStructs.h"

class PostEffectManager
{
public:
	PostEffectManager();
	void Update();
public:

	CBufferDataBloom bloomData_{};
	CBufferDataVignette vignetteData_{};

	// ダッシュのエフェクト
	DashBlur dashEffect_{};

};
