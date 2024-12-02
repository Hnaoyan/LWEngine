#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include <stdint.h>

class GPUParticleEmitter;

namespace BossSystemContext
{
	// 追尾の種類
	enum class TrackType
	{
		kStandard,	// 盆栽
		kInferior,	// 劣等
		kSuperior,	// 優等
	};	

	// エフェクトのパラメータ
	struct EffectParams
	{
		// エミッターのポインタ
		GPUParticleEmitter* emitPtr = nullptr;
		// アクティブフラグ
		bool isActive = false;
		// テクスチャハンドル
		uint32_t texture = 0u;
		// 
		FrameTimer activeTimer;
		float activeFrame;
	};
}
