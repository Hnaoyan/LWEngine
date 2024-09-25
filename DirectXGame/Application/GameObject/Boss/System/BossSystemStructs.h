#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include <stdint.h>

class ParticleEmitter;

namespace BossSystemContext
{
	struct DamageParams
	{
		// エミッターのポインタ
		ParticleEmitter* emitPtr = nullptr;
		// アクティブフラグ
		bool isActive = false;
		// テクスチャハンドル
		uint32_t texture = 0u;
		// 
		FrameTimer activeTimer;
		float activeFrame;
	};
}
