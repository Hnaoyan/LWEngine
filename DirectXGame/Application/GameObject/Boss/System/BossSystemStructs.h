#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include <stdint.h>

class ParticleEmitter;

namespace BossSystemContext
{
	// 追尾の種類
	enum class TrackType
	{
		kStandard,	// 盆栽
		kInferior,	// 劣等
		kSuperior,	// 優等
	};	

	// ダメージ受けた時のパラメータ
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
