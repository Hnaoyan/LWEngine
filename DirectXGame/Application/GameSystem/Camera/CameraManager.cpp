#include "CameraManager.h"

CameraManager::CameraManager()
{
	followCamera_ = std::make_unique<FollowCamera>();
	debugCamera_ = std::make_unique<DebugCamera>();
}

void CameraManager::Initialize()
{
	// 初期化
	followCamera_->Initialize();
	debugCamera_->Initialize();
}

void CameraManager::Update()
{
	// 更新
	followCamera_->Update();
	debugCamera_->Update();
}
