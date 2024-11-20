#pragma once
#include "Engine/Camera/CameraList.h"
#include <memory>

class CameraSwitcher
{
public:

private:
	// 現在アクティブなカメラ
	ActiveCameraMode activeCamera_ = ActiveCameraMode::kNormal;

};
