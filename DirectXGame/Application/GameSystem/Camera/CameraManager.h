#pragma once
#include "Engine/Camera/CameraList.h"
#include <optional>
#include <memory>

class GameObjectManager;

class CameraManager
{
public:
	CameraManager();
	~CameraManager() = default;

public:
	void Initialize(GameObjectManager* gameManager);
	void Update();
	void ImGuiDraw();

	void ChangeCamera(ActiveCameraMode mode);

private:
	
	void UpdateCameraSwitcher();

public:
	ICamera* GetCamera();
	FollowCamera* GetFollowCamera() { return followCamera_.get(); }

private:
	// オブジェクトマネージャ
	GameObjectManager* gameObjManager_ = nullptr;

private:
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 注視カメラ
	std::unique_ptr<FocusCamera> focusCamera_;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	// 変更のリクエスト
	std::optional<ActiveCameraMode> changeRequest_ = std::nullopt;
	// 現在アクティブなカメラ
	ActiveCameraMode activeCamera_ = ActiveCameraMode::kNormal;
};
