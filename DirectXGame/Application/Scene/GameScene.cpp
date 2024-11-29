#include "GameScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

#include <imgui.h>

void GameScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// ライト初期化
	LightingInitialize();

#pragma region インスタンス化
	cameraManager_ = std::make_unique<CameraManager>();
	gpuParticleManager_ = std::make_unique<GPUParticleManager>();
	uiManager_ = std::make_unique<GameUIManager>();

	collisionManager_ = std::make_unique<CollisionManager>();
	gameObjectManager_ = std::make_unique<GameObjectManager>();
	gameSystem_ = std::make_unique<GameSystem>();
#pragma endregion

#pragma region システム
	// パーティクル
	gpuParticleManager_->Initialize(ModelManager::GetModel("Plane"));
	// ゲームシステム
	gameSystem_->Initialize();
	// UI
	uiManager_->Initialize();
	// コライダー
#pragma endregion
	// 準備完了
	isSceneReady_ = true;
	

	gameObjectManager_->Initialize(gpuParticleManager_.get(), cameraManager_->GetFollowCamera());
	cameraManager_->Initialize(gameObjectManager_.get());

#ifdef RELEASE
	nowState_ = GameSceneState::kGamePlay;
#endif // RELEASE


}

void GameScene::Update()
{
#ifdef IMGUI_ENABLED
	if (input_->TriggerKey(DIK_UPARROW)) {
		sceneManager_->ChangeScene("TITLE");
	}
#endif // _DEBUG

	// ステートの切り替わり処理
	ChangeState();

	// シーンの切り替え処理
	if (gameObjectManager_->IsSceneChange()) {
		sceneManager_->ChangeScene("TITLE");
		return;
	}

	// ライトの更新
	LightingUpdate();
	//---ゲームのシステム更新---//
	gameSystem_->Update();
	gameObjectManager_->Update();
	// 衝突処理
	CollisionUpdate();
	// カメラの更新
	CameraUpdate();

	gpuParticleManager_->Update();

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();

#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	ModelRenderer::PreDraw(commandList);

	// ライトの情報
	DrawDesc::LightDesc lightDesc{};
	lightDesc.directionalLight = directionalLight_.get();
	lightDesc.pointLight = pointLight_.get();
	lightDesc.spotLight = spotLight_.get();

	// オブジェクト
	gameObjectManager_->Draw(&camera_, lightDesc);

	// パーティクル
	gpuParticleManager_->Draw(&camera_);

	ModelRenderer::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion
}

void GameScene::UIDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	// ゲームの
	gameObjectManager_->UIDraw();

	// UI全般
	uiManager_->Draw(gameObjectManager_.get());

	Sprite::PostDraw();

}

void GameScene::ImGuiDraw()
{
#ifdef IMGUI_ENABLED
	gameSystem_->ImGuiDraw();
	gameObjectManager_->ImGuiDraw();
	cameraManager_->ImGuiDraw();
	// カメラ
	camera_.ImGuiDraw();

	ImGui::Begin("GameScene");
	if (ImGui::Button("Restart")) {
		stateRequest_ = GameSceneState::kGameRestart;
	}
	if (ImGui::Button("ReplayStart")) {
		stateRequest_ = GameSceneState::kReplay;
	}
	if (ImGui::Button("RecordingStart")) {
		stateRequest_ = GameSceneState::kRecord;
	}
	if (ImGui::Button("RecordingEnd")) {
		gameSystem_->GetReplayManager()->ExportReplay();
	}
	if (ImGui::BeginTabBar("Lighting"))
	{
		float defaultSpeed = 0.01f;
		if (ImGui::BeginTabItem("DirectionalLight"))
		{
			ImGui::ColorEdit4("Color", &lightData_.color.x);
			ImGui::DragFloat3("Direction", &lightData_.direction.x, defaultSpeed);
			lightData_.direction = Vector3::Normalize(lightData_.direction);
			ImGui::DragFloat("Intensity", &lightData_.intensity, defaultSpeed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("PointLight"))
		{
			ImGui::ColorEdit4("ptColor", &ptLightData_.color.x);
			ImGui::DragFloat("ptDecay", &ptLightData_.decay, defaultSpeed);
			ImGui::DragFloat("ptIntensity", &ptLightData_.intensity, defaultSpeed);
			ImGui::DragFloat("ptRadius", &ptLightData_.radius, defaultSpeed);
			ImGui::DragFloat3("ptPosition", &ptLightData_.position.x, defaultSpeed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("SpotLight"))
		{
			ImGui::ColorEdit4("spColor", &spLightData_.color.x);
			ImGui::DragFloat("spDecay", &spLightData_.decay, defaultSpeed);
			ImGui::DragFloat("spIntensity", &spLightData_.intensity, defaultSpeed);
			ImGui::DragFloat("spCosAngle", &spLightData_.cosAngle, defaultSpeed);
			ImGui::DragFloat("spCosFalloffStart", &spLightData_.cosFalloffStart, defaultSpeed);
			ImGui::DragFloat("spDistance", &spLightData_.distance, defaultSpeed);
			ImGui::DragFloat3("spPosition", &spLightData_.position.x, defaultSpeed);
			ImGui::DragFloat3("spDirection", &spLightData_.direction.x, defaultSpeed);
			spLightData_.direction = Vector3::Normalize(spLightData_.direction);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();


#endif // IMGUI_ENABLED
}

void GameScene::LoadModel()
{
	// モデルのロード
	ModelManager::LoadNormalModel("Terrain", "terrain");	// 地形
	ModelManager::LoadNormalModel("Enemy", "EnemyBug");	// 雑魚敵
	ModelManager::LoadNormalModel("BossEnemy", "Prizm");	// ボス
	ModelManager::LoadNormalModel("Player", "Robotto");	// プレイヤー
	ModelManager::LoadNormalModel("SkyDome", "SkyDome");	// 天球
	ModelManager::LoadNormalModel("BarrierSphere", "sphere");	// ボスのバリア
	ModelManager::LoadNormalModel("ParticleCube", "ParticleCube");	// パーティクル用のキューブ
	ModelManager::LoadNormalModel("TrailCube", "ParticleCube");	// 軌跡用のキューブ
	ModelManager::LoadNormalModel("BombPlane", "plane");	// 板ポリ
}

void GameScene::LoadTexture()
{
	// テクスチャのロード
	TextureManager::Load("Resources/UI/ClearText.png");
	TextureManager::Load("Resources/UI/DashUI.png");
	TextureManager::Load("Resources/UI/JumpUI.png");
	TextureManager::Load("Resources/UI/LockonUI.png");
	TextureManager::Load("Resources/UI/ShotUIt.png");
	TextureManager::Load("Resources/crossHair.png");
	TextureManager::Load("Resources/default/testGage.png");
	TextureManager::Load("Resources/UI/GameOver.png");
	TextureManager::Load("Resources/default/BackGround.png");

	// スプライトのロード
	SpriteManager::LoadSprite("CrossHair", TextureManager::Load("Resources/crossHair.png"));
	SpriteManager::LoadSprite("Gage", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("PlayerGage", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("PlayerEnergyGage", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("HPBackUI", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("PlayerHPBackUI", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("PlayerEnergyBackUI", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("GageBack", TextureManager::Load("Resources/default/testGage.png"));
	SpriteManager::LoadSprite("GameClearText", TextureManager::Load("Resources/UI/ClearText.png"));
	SpriteManager::LoadSprite("GameOverText", TextureManager::Load("Resources/UI/GameOver.png"));

}

void GameScene::CameraUpdate()
{
	// カメラの管理クラス
	cameraManager_->Update();

	// シーンで使用するカメラの更新
	camera_.viewMatrix_ = cameraManager_->GetCamera()->viewMatrix_;
	camera_.projectionMatrix_ = cameraManager_->GetCamera()->projectionMatrix_;
	camera_.transform_ = cameraManager_->GetCamera()->transform_;
	camera_.TransferMatrix();
}

void GameScene::LightingInitialize()
{
	// ライト作成
	directionalLight_.reset(DirectionalLight::CreateLight());
	pointLight_.reset(PointLight::CreateLight());
	spotLight_.reset(SpotLight::CreateLight());

	// 平行光源データ
	lightData_.color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_.direction = { 0.0f,1.0f,0.0f };
	lightData_.intensity = 1.2f;

	// 点光源データ
	ptLightData_.intensity = 0.5f;
	ptLightData_.position = { 0,50.0f,0 };
	ptLightData_.color = { 1,1,1,1 };
	ptLightData_.decay = 10.0f;
	ptLightData_.radius = 300.0f;

	// 照光源データ
	spLightData_.color = { 1,1,1,1 };
	spLightData_.position = { 2.0f,200.0f,0.0f };
	spLightData_.distance = 300.0f;
	spLightData_.direction = Vector3(-0.707f, -0.707f, 0.0f);
	spLightData_.intensity = 12.5f;
	spLightData_.decay = 3.0f;
	//spLightData_.cosAngle = std::cosf(std::numbers::pi_v<float> / 3.0f);
	spLightData_.cosAngle = 0.3f;
	spLightData_.cosFalloffStart = 0.5f;
}

void GameScene::LightingUpdate()
{
	// ライトの更新
	directionalLight_->Update(lightData_);
	spotLight_->Update(spLightData_);
	pointLight_->Update(ptLightData_);
}

void GameScene::CollisionUpdate()
{
	// クリア
	collisionManager_->ListClear();

	// ゲームの登録
	gameObjectManager_->RegisterCollider(collisionManager_.get());

	// 衝突処理
	collisionManager_->CheckAllCollisions();
}

void GameScene::ChangeState()
{
	if (stateRequest_) {
		nowState_ = stateRequest_.value();
		switch (nowState_)
		{
		case GameSceneState::kWait:

			break;
		case GameSceneState::kGameRestart:
			// オブジェクト類の初期化
			gpuParticleManager_->DataReset();	// パーティクルのリセット（これのせいでたぶんDebug動いてない
			gameObjectManager_->Initialize(gpuParticleManager_.get(), cameraManager_->GetFollowCamera());
			gameObjectManager_->GameSetUp();	// ゲームの準備
			cameraManager_->Initialize(gameObjectManager_.get());
			cameraManager_->ChangeCamera(ActiveCameraMode::kFollow);
			break;
		case GameSceneState::kGamePlay:

			break;
		case GameSceneState::kReplay:
			gameSystem_->GetReplayManager()->ImportReplay();
			// インスタンス生成しなおし
			//cameraManager_ = std::make_unique<CameraManager>();

			gameObjectManager_ = std::make_unique<GameObjectManager>();
			// オブジェクト類の初期化
			gpuParticleManager_->DataReset();	// パーティクルのリセット（これのせいでたぶんDebug動いてない
			gameObjectManager_->Initialize(gpuParticleManager_.get(), cameraManager_->GetFollowCamera());
			gameObjectManager_->GameSetUp();	// ゲームの準備
			cameraManager_->Initialize(gameObjectManager_.get());
			cameraManager_->ChangeCamera(ActiveCameraMode::kFollow);
			gameSystem_->LaunchReplay();
			break;
		case GameSceneState::kRecord:
			// インスタンス生成しなおし
			gameObjectManager_ = std::make_unique<GameObjectManager>();
			// オブジェクト類の初期化
			gpuParticleManager_->DataReset();	// パーティクルのリセット（これのせいでたぶんDebug動いてない
			gameObjectManager_->Initialize(gpuParticleManager_.get(), cameraManager_->GetFollowCamera());
			gameObjectManager_->GameSetUp();	// ゲームの準備
			cameraManager_->Initialize(gameObjectManager_.get());
			cameraManager_->ChangeCamera(ActiveCameraMode::kFollow);
			gameSystem_->GetReplayManager()->RecordSetUp();	// 記録のスタート処理

			break;
		}

		stateRequest_ = std::nullopt;
	}
}
