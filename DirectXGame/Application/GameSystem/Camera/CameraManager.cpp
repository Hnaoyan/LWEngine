#include "CameraManager.h"
#include "../GameObjectManager.h"

#include <imgui.h>

CameraManager::CameraManager()
{
	followCamera_ = std::make_unique<FollowCamera>();
	focusCamera_ = std::make_unique<FocusCamera>();
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
	// 注視点カメラ
	focusCamera_->Initialize();
	focusCamera_->transform_.translate = { 50.0f,0.0f,0.0f };
	focusCamera_->SetFocusPoint(&gameManager->GetPlayer()->worldTransform_.transform_.translate);
	focusCamera_->SetWorldTransform(gameObjManager_->GetPlayer()->GetWorldTransform());
	// 選択
	activeCamera_ = ActiveCameraMode::kFollow;

}

void CameraManager::Update()
{
	// 更新
	followCamera_->Update();
	focusCamera_->Update();
	debugCamera_->Update();

	if (changeRequest_) {
		ActiveCameraMode value = changeRequest_.value();
		activeCamera_ = value;
		switch (value)
		{
		case ActiveCameraMode::kNormal:
			activeCamera_ = value;
			break;
		case ActiveCameraMode::kFollow:
			break;
		case ActiveCameraMode::kFocus:
			break;
		case ActiveCameraMode::kDebug:
			break;
		case ActiveCameraMode::kMaxSize:
			break;
		}
		changeRequest_ = std::nullopt;
	}

}

void CameraManager::ImGuiDraw()
{
	ImGui::Begin("CameraManager");

	ImGui::SeparatorText("Switcher");
	if (ImGui::Button("Follow")) {
		ChangeCamera(ActiveCameraMode::kFollow);
	}
	if (ImGui::Button("Focus")) {
		ChangeCamera(ActiveCameraMode::kFocus);
	}
	if (ImGui::Button("Debug")) {
		ChangeCamera(ActiveCameraMode::kDebug);
	}

	if (ImGui::BeginTabBar("System"))
	{
		if (ImGui::BeginTabItem("Follow")) {
			followCamera_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Focus")) {
			focusCamera_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Debug")) {
			debugCamera_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void CameraManager::ChangeCamera(ActiveCameraMode mode)
{
	// リクエストを送信
	changeRequest_ = mode;
	// 何か追加することがあれば
	switch (mode)
	{
	case ActiveCameraMode::kNormal:
		break;
	case ActiveCameraMode::kFollow:
		break;
	case ActiveCameraMode::kFocus:
		break;
	case ActiveCameraMode::kDebug:
		break;
	case ActiveCameraMode::kMaxSize:
		break;
	default:
		break;
	}

}

void CameraManager::UpdateCameraSwitcher()
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
	case ActiveCameraMode::kFocus:
		return focusCamera_.get();
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
