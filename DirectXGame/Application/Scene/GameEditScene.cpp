#include "GameEditScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

#include <imgui.h>

void GameEditScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// ライト初期化
	LightingInitialize();
#pragma region カメラ関係
	// 初期カメラ
	camera_.transform_.translate.y = 5.0f;
	camera_.transform_.rotate.x = 0.4f;
	camera_.transform_.translate.z = -7.0f;
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
#pragma endregion
	// インスタンス生成
	gpuParticleManager_ = std::make_unique<GPUParticleManager>();
	boss_ = std::make_unique<Boss>();

	// パーティクル
	gpuParticleManager_->Initialize(ModelManager::GetModel("Plane"));

	boss_->SetGPUParticle(gpuParticleManager_.get());
	boss_->Initialize(ModelManager::GetModel("BossEnemy"));

}

void GameEditScene::GPUUpdate()
{
	gpuParticleManager_->Update();
}

void GameEditScene::Update()
{
	if (boss_) {
		boss_->Update();
	}
	// カメラの更新
	CameraUpdate();
}

void GameEditScene::Draw()
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
	ModelRenderer::PreDraw(commandList);
	ModelDrawDesc desc{};

	DrawDesc::LightDesc lightDesc{};
	lightDesc.directionalLight = directionalLight_.get();
	lightDesc.pointLight = pointLight_.get();
	lightDesc.spotLight = spotLight_.get();

	desc.camera = &camera_;
	desc.directionalLight = directionalLight_.get();
	desc.pointLight = pointLight_.get();
	desc.spotLight = spotLight_.get();
	if (boss_) {
		boss_->Draw(desc);
	}

	gpuParticleManager_->Draw(&camera_);
	ModelRenderer::PostDraw();
	Model::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion
}

void GameEditScene::UIDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();
}

void GameEditScene::ImGuiDraw()
{

}

void GameEditScene::LoadModel()
{
	// モデルのロード
	ModelManager::LoadNormalModel("Terrain", "terrain");
	ModelManager::LoadNormalModel("Jett", "jett");
	ModelManager::LoadNormalModel("Enemy", "EnemyBug");
	ModelManager::LoadNormalModel("BossEnemy", "Prizm");
	ModelManager::LoadNormalModel("Player", "Robotto");
	ModelManager::LoadNormalModel("SkyDome", "SkyDome");
	ModelManager::LoadNormalModel("BarrierSphere", "sphere");
}

void GameEditScene::LoadTexture()
{
	// テクスチャのロード
	int loadTex = TextureManager::Load("Resources/UI/ClearText.png");
	loadTex = TextureManager::Load("Resources/UI/DashUI.png");
	loadTex = TextureManager::Load("Resources/UI/JumpUI.png");
	loadTex = TextureManager::Load("Resources/UI/LockonUI.png");
	loadTex = TextureManager::Load("Resources/UI/ShotUI.png");

	loadTex = TextureManager::Load("Resources/crossHair.png");
	loadTex = TextureManager::Load("Resources/default/testGage.png");
	loadTex = TextureManager::Load("Resources/UI/GameOver.png");
	loadTex = TextureManager::Load("Resources/default/BackGround.png");

	uint32_t reticle = TextureManager::Load("Resources/crossHair.png");
	SpriteManager::LoadSprite("CrossHair", reticle);
	reticle = TextureManager::GetInstance()->Load("Resources/default/white2x2.png");
	SpriteManager::LoadSprite("Gage", reticle);
	SpriteManager::LoadSprite("PlayerGage", reticle);
	SpriteManager::LoadSprite("PlayerEnergyGage", reticle);
	reticle = TextureManager::GetInstance()->Load("Resources/default/white2x2.png");
	SpriteManager::LoadSprite("HPBackUI", reticle);
	SpriteManager::LoadSprite("PlayerHPBackUI", reticle);
	SpriteManager::LoadSprite("PlayerEnergyBackUI", reticle);
	reticle = TextureManager::GetInstance()->Load("Resources/default/testGage.png");
	SpriteManager::LoadSprite("GageBack", reticle);
	reticle = TextureManager::GetInstance()->Load("Resources/UI/GameOver.png");
	SpriteManager::LoadSprite("GameOverUI", reticle);
}

void GameEditScene::CameraUpdate()
{
	if (isDebugCamera_) {
		debugCamera_->Update();
		camera_.viewMatrix_ = debugCamera_->viewMatrix_;
		camera_.projectionMatrix_ = debugCamera_->projectionMatrix_;
		camera_.TransferMatrix();
		//camera_.Update();
	}
	else {
		followCamera_->Update();
		camera_.viewMatrix_ = followCamera_->viewMatrix_;
		camera_.projectionMatrix_ = followCamera_->projectionMatrix_;
		camera_.transform_ = followCamera_->transform_;
		camera_.TransferMatrix();
	}
}

void GameEditScene::LightingInitialize()
{
	// ライト作成
	directionalLight_.reset(DirectionalLight::CreateLight());
	pointLight_.reset(PointLight::CreateLight());
	spotLight_.reset(SpotLight::CreateLight());

	// 点光源データ
	ptLightData_.intensity = 1.0f;
	ptLightData_.position = { 0,2.0f,0 };
	ptLightData_.color = { 1,1,1,1 };
	ptLightData_.decay = 1.0f;
	ptLightData_.radius = 2.0f;

	// 照光源データ
	spLightData_.color = { 1,1,1,1 };
	spLightData_.position = { 2.0f,1.25f,0.0f };
	spLightData_.distance = 7.0f;
	spLightData_.direction = Vector3::Normalize({ -1.0f,-1.0f,0.0f });
	spLightData_.intensity = 4.0f;
	spLightData_.decay = 2.0f;
	spLightData_.cosAngle = std::cosf(std::numbers::pi_v<float> / 3.0f);
}

void GameEditScene::LightingUpdate()
{
}
