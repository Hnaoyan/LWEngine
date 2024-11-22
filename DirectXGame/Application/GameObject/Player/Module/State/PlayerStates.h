#pragma once
#include "IPlayerState.h"
// 水平
#include "Horizontal/IdleHorizontal.h"
#include "Horizontal/QuickBoostState.h"
#include "Horizontal/BoostState.h"
#include "Horizontal/MovingState.h"
// 垂直
#include "Vertical/IdleVertical.h"
#include "Vertical/AssendingState.h"
#include "Vertical/FallingState.h"
#include "Vertical/JumpingState.h"

enum class PlayerStateLists {
	kIdleVertical,	// 垂直の待機
	kIdleHorizontal,	// 水平の待機
	kMove, // 通常移動
	kJump, // ジャンプ
	kFall, // 落下
	kBoost, // 早い移動
	kQuickBoost, // ダッシュ
	kAssending, // 上昇
};
