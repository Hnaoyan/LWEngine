#include "CameraManager.h"
#include "../GameObjectManager.h"
#include "../GameSystem.h"

#include <imgui.h>

CameraManager::CameraManager()
{
	followCamera_ = std::make_unique<FollowCamera>();
	focusCamera_ = std::make_unique<FocusCamera>();
	debugCamera_ = std::make_unique<DebugCamera>();
	orbitCamera_ = std::make_unique<OrbitCamera>();
	sideCamera_ = std::make_unique<SideCamera>();
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
	// 半円カメラ
	orbitCamera_->Initialize();
	orbitCamera_->SetObject(&gameManager->GetPlayer()->worldTransform_.transform_.translate, &gameManager->GetBoss()->worldTransform_.transform_.translate);
	
	// 選択
	activeCamera_ = ActiveCameraMode::kFollow;

	sideCamera_->Initialize();
	sideCamera_->SetParent(&gameManager->GetPlayer()->worldTransform_);

}

void CameraManager::Update(GameSystem* gameSystem)
{
	// 更新
	followCamera_->Update();
	focusCamera_->Update();
	debugCamera_->Update();
	orbitCamera_->Update();
	sideCamera_->Update();

	durationTimer_.Update();

	// リプレイ中のカメラ切り替え
	if (gameSystem->IsReplayMode()) {
		if (!durationTimer_.IsActive()) {
			UpdateCameraSwitcher();
			// 長押しの重なりを回避するためのタイマー
			durationTimer_.Start(10.0f);
		}
	}
	// リプレイでない場合
	else {
		//changeRequest_ = ActiveCameraMode::kSide;
	}

	// リクエストによる変更
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
	if (ImGui::Button("Orbit")) {
		ChangeCamera(ActiveCameraMode::kOrbit);
	}
	if (ImGui::Button("Side")) {
		ChangeCamera(ActiveCameraMode::kSide);
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
		if (ImGui::BeginTabItem("Orbit")) {
			orbitCamera_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Side")) {
			sideCamera_->ImGuiDraw();
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
	Input* input = Input::GetInstance();

	if (input->XTriggerJoystick(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		switch (activeCamera_)
		{
		case ActiveCameraMode::kNormal:

			break;
		case ActiveCameraMode::kFollow:
			ChangeCamera(ActiveCameraMode::kFocus);
			break;
		case ActiveCameraMode::kFocus:
			ChangeCamera(ActiveCameraMode::kOrbit);
			break;
		case ActiveCameraMode::kOrbit:
			ChangeCamera(ActiveCameraMode::kFollow);
			break;
		case ActiveCameraMode::kSide:
			ChangeCamera(ActiveCameraMode::kSide);
			break;
		case ActiveCameraMode::kDebug:
			break;
		case ActiveCameraMode::kMaxSize:
			break;
		default:
			break;
		}
	}
	else if (input->XTriggerJoystick(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		switch (activeCamera_)
		{
		case ActiveCameraMode::kNormal:

			break;
		case ActiveCameraMode::kFollow:
			ChangeCamera(ActiveCameraMode::kOrbit);
			break;
		case ActiveCameraMode::kFocus:
			ChangeCamera(ActiveCameraMode::kFollow);
			break;
		case ActiveCameraMode::kOrbit:
			ChangeCamera(ActiveCameraMode::kFocus);
			break;
		case ActiveCameraMode::kSide:
			ChangeCamera(ActiveCameraMode::kSide);
			break;
		case ActiveCameraMode::kDebug:
			break;
		case ActiveCameraMode::kMaxSize:
			break;
		default:
			break;
		}
	}

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
	case ActiveCameraMode::kOrbit:
		return orbitCamera_.get();
		break;
	case ActiveCameraMode::kSide:
		return sideCamera_.get();
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
