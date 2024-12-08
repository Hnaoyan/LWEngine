#pragma once
// インクルードパス
#include "Core/ICamera.h"
#include "Custom/DebugCamera.h"
#include "Custom/FollowCamera.h"
#include "Custom/FocusCamera.h"
#include "Custom/OrbitCamera.h"
#include "Custom/SideCamera.h"
#include "Custom/TransitionCamera.h"

/// <summary>
/// アクティブ中のカメラ選択
/// </summary>
enum class ActiveCameraMode
{
	kNormal,
	kFollow,
	kFocus,
	kOrbit,
	kSide,
	kTransition,
	kDebug,
	kMaxSize,
};
