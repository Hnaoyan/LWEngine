#include "TitleTransitionManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Scene/SceneManager.h"
#include "imgui.h"

void TitleTransitionManager::Execute(const float& frame)
{
	timer_.Start(frame);
}

void TitleTransitionManager::TransitionExecute()
{
}

void TitleTransitionManager::Initialize(SceneManager* sceneManager)
{
	sceneManager_ = sceneManager;
	robotObject_ = std::make_unique<TitleObject>();
	robotObject_->Initialize(ModelManager::GetModel("Player"));

	camera_ = std::make_unique<TitleCamera>();
	camera_->Initialize();

	// ここら辺のマジックナンバー修正（後日
	camera_->transform_.translate.y = 1.5f;
	camera_->transform_.translate.z = -10.0f;

	cameraPoint_.start = { 0.0f,1.5f,-10.0f };
	cameraPoint_.end = { 0.0f,1.5f,-5.0f };

	cameras_pool = std::make_unique<ObjectPool<ICamera, 5>>();
}

void TitleTransitionManager::Update()
{
	timer_.Update();
	robotObject_->Update();

	if (timer_.IsActive()) {
		camera_->transform_.translate = Ease::Easing<Vector3>(cameraPoint_.start, cameraPoint_.end, timer_.GetElapsedFrame());
	}
	if (timer_.IsEnd()) {
		sceneManager_->ChangeThreadScene("GAME");
	}

	// カメラの更新
	camera_->Update();
}

void TitleTransitionManager::Draw(ICamera* camera, DrawDesc::LightDesc lights)
{
	ModelDrawDesc drawDesc{};
	drawDesc.camera = camera;
	// ライトの情報
	drawDesc.directionalLight = lights.directionalLight;
	drawDesc.pointLight = lights.pointLight;
	drawDesc.spotLight = lights.spotLight;

	// 描画
	//robotObject_->Draw(drawDesc);
}

void TitleTransitionManager::ImGuiDraw()
{
	ImGui::Begin("Transition");

	static float dragValue = 0.01f;
	ImGui::DragFloat("DragV", &dragValue, 0.1f);
	ImGui::DragFloat3("CameraPos", &camera_->transform_.translate.x, dragValue);
	ImGui::DragFloat3("CameraRot", &camera_->transform_.rotate.x, dragValue);

	ImGui::End();

	robotObject_->ImGuiDraw();

}
