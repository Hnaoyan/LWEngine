#include "TitleTransitionManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Scene/SceneManager.h"

void TitleTransitionManager::Execute(const float& frame)
{
	timer_.Start(frame);
}

void TitleTransitionManager::Initialize(SceneManager* sceneManager)
{
	sceneManager_ = sceneManager;
	robotObject_ = std::make_unique<TitleObject>();
	robotObject_->Initialize(ModelManager::GetModel("Player"));

}

void TitleTransitionManager::Update()
{
	timer_.Update();
	robotObject_->Update();

	if (timer_.IsEnd()) {
		sceneManager_->ChangeThreadScene("GAME");
	}

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
	robotObject_->Draw(drawDesc);
}
