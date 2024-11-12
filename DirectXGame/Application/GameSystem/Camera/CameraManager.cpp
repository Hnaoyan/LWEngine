#include "CameraManager.h"

CameraManager::CameraManager()
{
	followCamera_ = std::make_unique<FollowCamera>();
}

void CameraManager::Update()
{
	followCamera_->Update();
}
