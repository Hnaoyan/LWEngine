#include "TitleScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/2D/SpriteManager.h"

#include <imgui.h>
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
	levelLoader_->LoadSceneData("01_10");

	TextureManager::sEnvironmentTexture = skybox_->GetTexture();
	SpriteManager::GetSprite("BackGroundTexture")->SetSize(Vector2((float)WindowAPI::kWindowWidth, (float)WindowAPI::kWindowHeight));
	SpriteManager::GetSprite("BackGroundTexture")->SetPosition(Vector2((float)WindowAPI::kWindowWidth / 2.0f, (float)WindowAPI::kWindowHeight / 2.0f));
	SpriteManager::GetSprite("BackGroundTexture")->SetAnchorPoint(Vector2(0.5f, 0.5f));

	SpriteManager::GetSprite("BackGroundImage")->SetPosition(Vector2(1280.0f / 2.0f, 720.0f / 2.0f));
	SpriteManager::GetSprite("TitleButtonText")->SetPosition(Vector2(1280.0f / 2.0f, 500.0f));
	SpriteManager::GetSprite("TitleButtonText")->SetSize(Vector2(240.0f, 120.0f) * 1.5f);
	SpriteManager::GetSprite("TitleText")->SetPosition(Vector2(1280.0f / 2.0f, 720.0f / 3.0f));
	titleTextPosition_ = Vector2(1280.0f / 2.0f, 720.0f / 3.0f);
	//loaderThread.join();  // スレッドの完了を待つ
	// 準備完了
	isSceneReady_ = true;
	isLoad_ = false;
	elapsedTime_ = 0.0f;
}

void TitleScene::GPUUpdate()
{

}

void TitleScene::Update()
{
#ifdef IMGUI_ENABLED
	//if (input_->TriggerKey(DIK_LEFTARROW)) {
	//	sceneManager_->ChangeThreadScene("SAMPLE");
	//}
	if (input_->TriggerKey(DIK_RIGHTARROW)) {
		sceneManager_->ChangeThreadScene("GAME");
		//isLoad_ = true;
	}
	//if (input_->TriggerKey(DIK_H)) {
	//	sceneManager_->ChangeThreadScene("GAMEEDIT");
	//}
#endif // IMGUI_ENABLED

	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_Y)) {
		sceneManager_->ChangeThreadScene("GAME");
		//isLoad_ = true;
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
	if (!isLoad_) {
		float defaultY = 720.0f / 3.0f;
		// 時間
		float addTime = 1.0f / 30.0f; // 毎フレーム加算する時間
		elapsedTime_ += addTime;
		float frequency = 1.0f;	// 間隔
		float amplitude = 10.0f;	// 振幅
		float maxTime = 1.0e6f;	// バグ回避用の最大時間
		if (elapsedTime_ >= maxTime) {
			elapsedTime_ = 0.0f;
		}
		float offset = std::sinf(elapsedTime_ * frequency) * amplitude;
		titleTextPosition_.y = defaultY + offset;
	}

	SpriteManager::GetSprite("TitleText")->SetPosition(titleTextPosition_);
	SpriteManager::GetSprite("TitleButtonText")->Draw();
	SpriteManager::GetSprite("TitleText")->Draw();
	//SpriteManager::GetSprite("BackGroundTexture")->Draw();
	SpriteManager::GetSprite("BackGroundImage")->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.85f));
	SpriteManager::GetSprite("BackGroundImage")->Draw();

	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::ImGuiDraw()
{
	ImGui::Begin("Title");
	ImGui::DragFloat2("TitleText", &titleTextPosition_.x, 0.01f);
	ImGui::End();

}

void TitleScene::LoadModel()
{
	//ModelManager::LoadObjModel("Plane", "plane");
	//ModelManager::LoadObjModel("Axis", "BulletTest");

}

void TitleScene::LoadTexture()
{
	skybox_.reset(Skybox::CreateSkybox("rostock_laage_airport_4k.dds"));
	SpriteManager::LoadSprite("BackGroundImage", TextureManager::GetInstance()->Load("Resources/UI/BackGround.png"));
	SpriteManager::LoadSprite("BackGroundTexture", TextureManager::GetInstance()->Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("TitleButtonText", TextureManager::GetInstance()->Load("Resources/UI/GameStart.png"));
	SpriteManager::LoadSprite("TitleText", TextureManager::GetInstance()->Load("Resources/UI/TitleText.png"));
	
	//clearTexture = TextureManager::GetInstance()->Load("Resources/UI/ShotUI.png");
}
