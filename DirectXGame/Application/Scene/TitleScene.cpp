#include "TitleScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/2D/SpriteManager.h"

#include <thread>
#include <functional>
#include <mutex>

void TitleScene::Initialize()
{
	IScene::Initialize();
	//std::mutex resourceMutex;
	//std::thread loaderThread([this, &resourceMutex]() {
	//	std::lock_guard<std::mutex> lock(resourceMutex);  // リソースへのアクセスをロック
	//	this->LoadModel();
	//	});	LoadModel();
	LoadModel();
	LoadTexture();

	levelLoader_ = std::make_unique<LevelLoader>();
	levelLoader_->LoadSceneData("01_12");

	TextureManager::sEnvironmentTexture = skybox_->GetTexture();

	SpriteManager::LoadSprite("BackGroundImage", TextureManager::GetInstance()->Load("Resources/UI/BackGround.png"));
	SpriteManager::LoadSprite("TitleButtonText", TextureManager::GetInstance()->Load("Resources/UI/Button.png"));
	SpriteManager::LoadSprite("TitleText", TextureManager::GetInstance()->Load("Resources/UI/TitleText.png"));

	SpriteManager::GetSprite("BackGroundImage")->SetPosition(Vector2(1280.0f / 2.0f, 720.0f / 2.0f));
	SpriteManager::GetSprite("TitleButtonText")->SetPosition(Vector2(1280.0f / 2.0f, 500.0f));
	SpriteManager::GetSprite("TitleButtonText")->SetSize(Vector2(240.0f, 120.0f) * 1.5f);
	SpriteManager::GetSprite("TitleText")->SetPosition(Vector2(1280.0f / 2.0f, 720.0f / 3.0f));

	//loaderThread.join();  // スレッドの完了を待つ
	// 準備完了
	isSceneReady_ = true;
}

void TitleScene::GPUUpdate()
{

}

void TitleScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneManager_->ChangeThreadScene("SAMPLE");
	}
	if (input_->TriggerKey(DIK_RSHIFT)) {
		sceneManager_->ChangeThreadScene("GAME");
	}
	if (input_->TriggerKey(DIK_H)) {
		sceneManager_->ChangeThreadScene("GAMEEDIT");
	}
#endif // _DEBUG

	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_Y)) {
		sceneManager_->ChangeThreadScene("GAME");
	}

}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景
	Sprite::PreDraw(commandList);

	//uiSprites_[0]->Draw();

	Sprite::PostDraw();
#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);

	Model::PostDraw();

}

void TitleScene::UIDraw()
{
#pragma region UI
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	SpriteManager::GetSprite("TitleButtonText")->Draw();
	SpriteManager::GetSprite("TitleText")->Draw();
	SpriteManager::GetSprite("BackGroundImage")->Draw();

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

}

void TitleScene::LoadTexture()
{
	skybox_.reset(Skybox::CreateSkybox("rostock_laage_airport_4k.dds"));
	//clearTexture = TextureManager::GetInstance()->Load("Resources/UI/ShotUI.png");
}
