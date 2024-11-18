#pragma once
#include "../GameSystemStructs.h"

class PostEffectManager
{
public:
	PostEffectManager();

public:

	CBufferDataBloom bloomData_{};
	CBufferDataVignette vignetteData_{};

	// ダッシュのエフェクト
	DashBlur dashEffect_{};

};
