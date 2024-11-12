#pragma once

#include "Core/ICamera.h"
#include "Custom/DebugCamera.h"
#include "Custom/FollowCamera.h"

enum class ActiveCameraMode
{
	kNormal,
	kFollow,
	kDebug,
	kMaxSize,
};
