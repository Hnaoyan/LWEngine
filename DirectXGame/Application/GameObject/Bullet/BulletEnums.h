#pragma once
#include <stdint.h>

// 追尾の種類
enum class BulletType : uint32_t
{
	kNormal,	// 通常
	kTracking,	// 追尾
};
// 追尾の種類
enum class TrackingType : uint32_t
{
	kStandard,	// 盆栽
	kInferior,	// 劣等
	kSuperior,	// 優等
};

enum class TrackingState : uint32_t
{
	kStraight,
	kWave,
	kTracking,
};
