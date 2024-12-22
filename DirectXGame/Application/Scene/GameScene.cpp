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
	lightManager_ = std::make_unique<LightingManager>();
	lightManager_->Initialize();

#pragma region インスタンス化
	cameraManager_ = std::make_unique<CameraManager>();
	gpuParticleManager_ = std::make_unique<GPUParticleManager>();

	collisionManager_ = std::make_unique<CollisionManager>();
	gameSystem_ = std::make_unique<GameSystem>();
	gameObjectManager_ = std::make_unique<GameObjectManager>(gameSystem_.get());
	uiManager_ = std::make_unique<GameUIManager>(gameSystem_.get());
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
	
	// オブジェクトマネージャ
	gameObjectManager_->Initialize(gpuParticleManager_.get(), cameraManager_->GetFollowCamera());
	cameraManager_->Initialize(gameObjectManager_.get());

	stateRequest_ = GameSceneState::kGameTutorial;

#ifdef RELEASE
	stateRequest_ = GameSceneState::kRecord;
	//nowState_ = GameSceneState::kGamePlay;
#endif // RELEASE

	this->ChangeState();

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
	// チュートリアルの状態から変更する処理
	if (this->nowState_ == GameSceneState::kGameTutorial) {
		if (input_->TriggerKey(DIK_SPACE)) {

#ifdef IMGUI_ENABLED
			stateRequest_ = GameSceneState::kGamePlay;
#endif // IMGUI_ENABLED
#ifdef RELEASE
			stateRequest_ = GameSceneState::kRecord;
#endif // RELEASE
		}
	}
	// シーンの切り替え処理
	if (gameObjectManager_->IsSceneChange()) {
		sceneManager_->ChangeScene("TITLE");
	}
	if (gameObjectManager_->IsGameEnd()) {
		// プレイの保存
		gameSystem_->GetReplayManager()->ExportReplay();
		gameObjectManager_->SetGameEnd(false);
	}
	if (gameObjectManager_->IsSceneReplay()) {
		// リプレイモードに移行
		stateRequest_ = GameSceneState::kReplay;
	}

	//---ゲームのシステム更新---//
	gameSystem_->Update();
	gameObjectManager_->Update();
	// 衝突処理
	CollisionUpdate();
	// カメラの更新
	CameraUpdate();
	// ライトの更新
	lightManager_->Update();

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
	lightDesc.directionalLight = lightManager_->GetDirectional();
	lightDesc.pointLight = lightManager_->GetPoint();
	lightDesc.spotLight = lightManager_->GetSpot();

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
	// UI全般
	uiManager_->Draw(gameObjectManager_.get());
	if (!gameSystem_->IsReplayMode()) {
		SpriteManager::GetSprite("UIBGTexture")->SetPosition(position_);
		SpriteManager::GetSprite("UIBGTexture")->SetSize(size_);
		SpriteManager::GetSprite("UIBGTexture")->SetColor(color_);
		SpriteManager::GetSprite("UIBGTexture")->Draw();
	}

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

	if (ImGui::TreeNode("Replayer")) {
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

		if (ImGui::Button("GameSetUp")) {
			BeginGame();
		}
		if (ImGui::Button("TutorialSetUp")) {
			BeginTutorial();
		}

		ImGui::TreePop();
	}
	ImGui::Separator();

	Vector2 rightStick = GameSystem::sKeyConfigManager.GetKeyConfig()->rightStick;
	Vector2 leftStick = GameSystem::sKeyConfigManager.GetKeyConfig()->leftStick;
	ImGui::DragFloat2("left", &leftStick.x);
	ImGui::DragFloat2("right", &rightStick.x);

	ImGui::Text("");
	// ライト
	lightManager_->ImGuiDraw();

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
	ModelManager::LoadNormalModel("PlayerOutline", "Robotto");	// コンボ用の枠モデル
	ModelManager::LoadNormalModel("SkyDome", "SkyDome");	// 天球
	ModelManager::LoadNormalModel("BarrierSphere", "sphere");	// ボスのバリア
	ModelManager::LoadNormalModel("ColliderSphere", "sphere");	// ボスのバリア
	ModelManager::LoadNormalModel("ParticleCube", "ParticleCube");	// パーティクル用のキューブ
	ModelManager::LoadNormalModel("TrailCube", "ParticleCube");	// 軌跡用のキューブ
	ModelManager::LoadNormalModel("BombPlane", "plane");	// 板ポリ
	ModelManager::LoadNormalModel("Plane", "plane");
}

void GameScene::LoadTexture()
{
	// テクスチャのロード
	TextureManager::Load("Resources/crossHair.png");
	TextureManager::Load("Resources/default/testGage.png");
	TextureManager::Load("Resources/default/BackGround.png");

	// スプライトのロード
	SpriteManager::LoadSprite("CrossHair", TextureManager::Load("Resources/crossHair.png"));	// クロスへア
	SpriteManager::LoadSprite("Gage", TextureManager::Load("Resources/default/white2x2.png"));	// ゲージ
	SpriteManager::LoadSprite("PlayerGage", TextureManager::Load("Resources/default/white2x2.png"));	// 
	SpriteManager::LoadSprite("PlayerDodgeGage", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("PlayerEnergyGage", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("HPBackUI", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("PlayerHPBackUI", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("PlayerEnergyBackUI", TextureManager::Load("Resources/default/white2x2.png"));
	SpriteManager::LoadSprite("GageBack", TextureManager::Load("Resources/default/testGage.png"));
	// テキスト関係
	SpriteManager::LoadSprite("GameClearText", TextureManager::Load("Resources/UI/GameClearText.png"));
	SpriteManager::LoadSprite("GameOverText", TextureManager::Load("Resources/UI/GameOverText.png"));
	SpriteManager::LoadSprite("ResultTitleUI", TextureManager::Load("Resources/UI/TitleBackUI.png"));
	SpriteManager::LoadSprite("ResultReplayUI", TextureManager::Load("Resources/UI/ReplayUI.png"));
	SpriteManager::LoadSprite("CameraChangeUI", TextureManager::Load("Resources/UI/CameraChangeUI.png"));

	SpriteManager::LoadSprite("UIBGTexture", TextureManager::Load("Resources/default/white2x2.png"));
}

void GameScene::CameraUpdate()
{
	// カメラの管理クラス
	cameraManager_->Update(gameSystem_.get());

	// シーンで使用するカメラの更新
	camera_.viewMatrix_ = cameraManager_->GetCamera()->viewMatrix_;
	camera_.projectionMatrix_ = cameraManager_->GetCamera()->projectionMatrix_;
	camera_.transform_ = cameraManager_->GetCamera()->transform_;
	camera_.TransferMatrix();
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

void GameScene::BeginGame()
{
	// オブジェクト類の初期化
	gpuParticleManager_->DataReset();	// パーティクルのリセット（これのせいでたぶんDebug動いてない
	gameObjectManager_->GameSetUp();	// ゲームの準備
	cameraManager_->GameSetUp();
	cameraManager_->ChangeCamera(ActiveCameraMode::kFollow);
}

void GameScene::BeginTutorial()
{
	gpuParticleManager_->DataReset();	// パーティクルのリセット（これのせいでたぶんDebug動いてない
	gameObjectManager_->TutorialSetUp();	// チュートリアルの準備
	cameraManager_->TutorialSetUp();
	cameraManager_->ChangeCamera(ActiveCameraMode::kFollow);
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
			BeginGame();
			break;
		case GameSceneState::kGamePlay:
			BeginGame();
			break;
		case GameSceneState::kGameTutorial:
			BeginTutorial();
			break;
		case GameSceneState::kReplay:
			gameSystem_->GetReplayManager()->ImportReplay();
			// UIを隠すフラグ
			uiManager_->SetIsHudHide(true);
			BeginGame();
			gameSystem_->LaunchReplay();
			break;
		case GameSceneState::kRecord:
			BeginGame();
			gameSystem_->GetReplayManager()->RecordSetUp();	// 記録のスタート処理
			break;
		}

		stateRequest_ = std::nullopt;
	}
}
