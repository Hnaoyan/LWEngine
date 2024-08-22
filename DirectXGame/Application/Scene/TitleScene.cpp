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

	AddSprite(Vector2(1280.0f / 2.0f, 720.0f / 2.0f), TextureManager::GetInstance()->Load("Resources/UI/BackGround.png"));
	AddSprite(Vector2(1280.0f / 2.0f, 720.0f / 2.0f), TextureManager::GetInstance()->Load("Resources/UI/Button.png"));

	size_ = uiSprites_[1]->GetSize();
	uiSprites_[1]->SetSize(size_ * 2);
}

void TitleScene::GPUUpdate()
{

}

void TitleScene::Update()
{
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneManager_->ChangeScene("SAMPLE");
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

	uiSprites_[0]->Draw();

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

	for (int i = 1; i < uiSprites_.size(); ++i) {
		uiSprites_[i]->Draw();
	}

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

void TitleScene::AddSprite(Vector2 position, uint32_t texture)
{
	std::unique_ptr<Sprite> instance;
	instance.reset(Sprite::Create(texture, position, { 0.5f,0.5f }));
	uiSprites_.push_back(std::move(instance));
}
