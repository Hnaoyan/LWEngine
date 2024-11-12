#include "CameraManager.h"
#include "../GameObjectManager.h"

CameraManager::CameraManager()
{
	followCamera_ = std::make_unique<FollowCamera>();
	debugCamera_ = std::make_unique<DebugCamera>();
}

void CameraManager::Initialize(GameObjectManager* gameManager)
{
	gameObjManager_ = gameManager;
	// 初期化
	// デバッグカメラ
	debugCamera_->Initialize();

	// 追従カメラ
	followCamera_->Initialize();
	followCamera_->SetParent(gameObjManager_->GetPlayer()->GetWorldTransform());
	followCamera_->SetLockOn(gameObjManager_->GetPlayer()->GetOperation()->GetLockOn());

	// 選択
	activeCamera_ = ActiveCameraMode::kFollow;

}

void CameraManager::Update()
{
	// 更新
	followCamera_->Update();
	debugCamera_->Update();
}

void CameraManager::ImGuiDraw()
{

}

ICamera* CameraManager::GetCamera()
{
	switch (activeCamera_)
	{
	case ActiveCameraMode::kNormal:
		break;
	case ActiveCameraMode::kFollow:
		return followCamera_.get();
		break;
	case ActiveCameraMode::kDebug:
		return debugCamera_.get();
		break;
	case ActiveCameraMode::kMaxSize:
		break;
	default:
		break;
	}
	return nullptr;
}
