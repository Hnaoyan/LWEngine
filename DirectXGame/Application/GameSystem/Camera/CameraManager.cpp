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
	transitionCamera_ = std::make_unique<TransitionCamera>();
}

void CameraManager::Initialize(GameObjectManager* gameManager)
{
	gameObjManager_ = gameManager;
	// 初期化
	// デバッグカメラ
	debugCamera_->Initialize();

	// 追従カメラ
	followCamera_->Initialize();
	// 注視点カメラ
	focusCamera_->Initialize();
	Vector3 focusCameraPosition = { 50.0f,0.0f,0.0f };	// カメラの座標
	focusCamera_->transform_.translate = focusCameraPosition;
	// 半円カメラ
	orbitCamera_->Initialize();

	sideCamera_->Initialize();

	transitionCamera_->Initialize();

	// 選択
	activeCamera_ = ActiveCameraMode::kFollow;
#ifdef IMGUI_ENABLED
	activeCamera_ = ActiveCameraMode::kDebug;
#endif // IMGUI_ENABLED


}

void CameraManager::Update(GameSystem* gameSystem)
{
	// 更新
	followCamera_->Update();
	focusCamera_->Update();
	debugCamera_->Update();
	orbitCamera_->Update();
	sideCamera_->Update();
	transitionCamera_->Update();

	// キー入力の間隔
	durationTimer_.Update();

	// リプレイ中のカメラ切り替え
	if (gameSystem->IsReplayMode()) {
		ReplayCameraSwitcher();
	}
	// ゲーム中のカメラ切り替え
	else {
		//InGameCameraSwitcher();
	}

	// リクエストによる変更
	if (changeRequest_) {
		ActiveCameraMode value = changeRequest_.value();
		// 一致している場合スキップ
		if (activeCamera_ == value) {
			return;
		}
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

void CameraManager::GameSetUp()
{
	// 追従カメラ
	followCamera_->SetParent(gameObjManager_->GetPlayer()->GetWorldTransform());
	followCamera_->SetLockOn(gameObjManager_->GetPlayer()->GetOperation()->GetLockOn());
	// 注視
	focusCamera_->SetFocusPoint(&gameObjManager_->GetPlayer()->worldTransform_.transform_.translate);
	// 半円
	orbitCamera_->SetObject(&gameObjManager_->GetPlayer()->worldTransform_.transform_.translate, &gameObjManager_->GetBoss()->worldTransform_.transform_.translate);
	// サイド
	sideCamera_->SetParent(&gameObjManager_->GetPlayer()->worldTransform_);

	// 追従カメラに変更
	this->activeCamera_ = ActiveCameraMode::kFollow;
}

void CameraManager::TutorialSetUp()
{
	// 追従カメラ
	followCamera_->SetParent(gameObjManager_->GetPlayer()->GetWorldTransform());
	followCamera_->SetLockOn(gameObjManager_->GetPlayer()->GetOperation()->GetLockOn());
	// 注視
	focusCamera_->SetFocusPoint(&gameObjManager_->GetPlayer()->worldTransform_.transform_.translate);
	// サイド
	sideCamera_->SetParent(&gameObjManager_->GetPlayer()->worldTransform_);
	// 追従カメラに変更
	this->activeCamera_ = ActiveCameraMode::kFollow;
}

void CameraManager::InGameCameraSwitcher()
{
	// プレイヤー
	Player* player = gameObjManager_->GetPlayer();
	// ジャスト回避中
	if (player->GetSystemFacede()->GetDudgeManager()->IsInvisibleActive()) {
		changeRequest_ = ActiveCameraMode::kSide;
	}
	// ジャスト回避外
	else {
		changeRequest_ = ActiveCameraMode::kFollow;
	}
}

void CameraManager::ReplayCameraSwitcher()
{
	if (!durationTimer_.IsActive()) {
		// 入力による切り替わり
		InputSwitch();
		// 長押しの重なりを回避するためのタイマー
		float durationFrame = 10.0f;
		durationTimer_.Start(durationFrame);
	}
}

void CameraManager::InputSwitch()
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
