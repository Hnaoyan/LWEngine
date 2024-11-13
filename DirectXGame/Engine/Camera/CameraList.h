#pragma once

#include "Core/ICamera.h"
#include "Custom/DebugCamera.h"
#include "Custom/FollowCamera.h"
#include "Custom/FocusCamera.h"

enum class ActiveCameraMode
{
	kNormal,
	kFollow,
	kFocus,
	kDebug,
	kMaxSize,
};
