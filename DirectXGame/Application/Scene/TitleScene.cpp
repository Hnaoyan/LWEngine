#include "TitleScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"

void TitleScene::Initialize()
{
	IScene::Initialize();
	LoadModel();
	LoadTexture();

	levelLoader_ = std::make_unique<LevelLoader>();
	levelLoader_->LoadSceneData("01_12");

	TextureManager::sEnvironmentTexture = skybox_->GetTexture();
}

void TitleScene::Update()
{
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneManager_->ChangeScene("GAME");
	}

#endif // _DEBUG

	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_Y)) {
		sceneManager_->ChangeScene("GAME");
	}

}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景
	Sprite::PreDraw(commandList);

	Sprite::PostDraw();
#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);

	Model::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);

	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::ImGuiDraw()
{

}

void TitleScene::LoadModel()
{
	ModelManager::LoadObjModel("Plane", "plane");
	ModelManager::LoadObjModel("Axis", "BulletTest");

	skybox_.reset(Skybox::CreateSkybox("rostock_laage_airport_4k.dds"));
}

void TitleScene::LoadTexture()
{
}
