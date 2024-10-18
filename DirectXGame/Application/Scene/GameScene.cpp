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
	// ゲームオーバーフラグ
	isGameOver_ = false;

#pragma region カメラ関係
	// 初期カメラ
	camera_.transform_.translate.y = 5.0f;
	camera_.transform_.rotate.x = 0.4f;
	camera_.transform_.translate.z = -7.0f;
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

#pragma endregion

#pragma region インスタンス化
	gpuParticleManager_ = std::make_unique<GPUParticleSystem>();
	uiManager_ = std::make_unique<GameUI::UIManager>();

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

	gameObjectManager_->Initialize(gpuParticleManager_.get());
}

void GameScene::GPUUpdate()
{
	gpuParticleManager_->Update();
}

void GameScene::Update()
{
	

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneManager_->ChangeScene("TITLE");
	}
#endif // _DEBUG
	//// 死亡チェック
	//if (player_->IsDead() && !isGameOver_) {
	//	backTitleTimer_.Start(120.0f);
	//	isGameOver_ = true;
	//}

	//// シーン変更の処理
	//if (isGameOver_) {
	//	backTitleTimer_.Update();
	//	if (!backTitleTimer_.IsActive()) {
	//		sceneManager_->ChangeScene("TITLE");
	//	}
	//	return;
	//}
	//if (clearText_.isClear) {
	//	clearText_.transitionTimer.Update();

	//	if (clearText_.transitionTimer.IsEnd()) {
	//		sceneManager_->ChangeScene("TITLE");
	//	}
	//}

	//---ゲームのシステム更新---//
	LightingUpdate();
	gameSystem_->Update();
	gameObjectManager_->Update();
	// 衝突処理
	CollisionUpdate();
	// カメラの更新
	CameraUpdate();
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
	uiManager_->Draw();

	if (clearText_.isClear) {
		clearText_.clearText->Draw();
	}
	if (isGameOver_) {
		SpriteManager::GetSprite("GameOverUI")->SetPosition({ 1280.0f / 2.0f,720.0f / 2.0f });
		SpriteManager::GetSprite("GameOverUI")->Draw();
	}
	for (int i = 0; i < controlUIs_.size(); ++i) {
		controlUIs_[i].first->SetPosition(controlUIs_[i].second.position);
		controlUIs_[i].first->Draw();
	}

	Sprite::PostDraw();

}

void GameScene::ImGuiDraw()
{
#ifdef IMGUI_ENABLED
	
	gameObjectManager_->ImGuiDraw();

	// カメラ
	camera_.ImGuiDraw();
	debugCamera_->ImGuiDraw();

	gpuParticleManager_->ImGuiDraw();

	ImGui::Begin("GameScene");
	if (ImGui::Button("PostDefault")) {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kAlpha;
	}
	if (ImGui::Button("PostBloom")) {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
	}
	ImGui::DragFloat("BloomThreshold", &gameSystem_->bloomData_.threshold, 0.01f);
	ImGui::DragFloat("BloomSigma", &gameSystem_->bloomData_.sigma, 0.01f);

	//if (ImGui::Button("BossRes")) {
	//	if (!bossEnemy_) {
	//		bossEnemy_ = std::make_unique<Boss>();
	//		bossEnemy_->Initialize(ModelManager::GetModel("DefaultCube"));
	//		bossEnemy_->SetPlayer(player_.get());
	//		player_->SetBoss(bossEnemy_.get());
	//	}
	//}
	//if (ImGui::Button("BossKill")) {
	//	if (bossEnemy_) {
	//		bossEnemy_->IsDead();
	//	}
	//}

	ImGui::Checkbox("DebugCamera", &isDebugCamera_);

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::TreePop();
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

	ImGui::Begin("UI");

	for (int i = 0; i < controlUIs_.size(); ++i) {
		ImGui::DragFloat2(controlUIs_[i].second.tag.c_str(), &controlUIs_[i].second.position.x, 1.0f);
		ImGui::Text("\n");
	}

	ImGui::End();

#endif // IMGUI_ENABLED
}

void GameScene::LoadModel()
{
	// モデルのロード
	ModelManager::LoadNormalModel("Terrain", "terrain");
	ModelManager::LoadNormalModel("Jett", "jett");
	ModelManager::LoadNormalModel("Enemy", "EnemyBug");
	ModelManager::LoadNormalModel("BossEnemy", "Prizm");
	ModelManager::LoadNormalModel("Player", "Robotto");
	ModelManager::LoadNormalModel("SkyDome", "SkyDome");
	ModelManager::LoadNormalModel("BarrierSphere", "sphere");
	ModelManager::LoadNormalModel("ParticleCube", "ParticleCube");
	ModelManager::LoadNormalModel("TrailCube", "ParticleCube");
}

void GameScene::LoadTexture()
{
	// テクスチャのロード
	int loadTex = TextureManager::GetInstance()->Load("Resources/UI/ClearText.png");
	loadTex = TextureManager::GetInstance()->Load("Resources/UI/DashUI.png");
	loadTex = TextureManager::GetInstance()->Load("Resources/UI/JumpUI.png");
	loadTex = TextureManager::GetInstance()->Load("Resources/UI/LockonUI.png");
	loadTex = TextureManager::GetInstance()->Load("Resources/UI/ShotUIt.png");

	loadTex = TextureManager::GetInstance()->Load("Resources/crossHair.png");
	loadTex = TextureManager::GetInstance()->Load("Resources/default/testGage.png");
	loadTex = TextureManager::GetInstance()->Load("Resources/UI/GameOver.png");
	loadTex = TextureManager::Load("Resources/default/BackGround.png");

	// テクスチャのロード
	clearText_.isClear = false;
	uint32_t clearTexture = TextureManager::GetInstance()->Load("Resources/UI/ClearText.png");
	clearText_.clearText.reset(Sprite::Create(clearTexture, { 1280.0f / 2.0f,720.0f / 2.0f }, { 0.5f,0.5f }));

	UIData data = {};
	data.num = uiNumber_;
	data.position = { 139.0f,60.0f };
	data.texture = TextureManager::GetInstance()->Load("Resources/UI/DashUI.png");
	data.tag = "UI" + std::to_string(uiNumber_);
	AddUI(data);
	data.num = uiNumber_;
	data.position = { 139.0f,180.0f };
	data.texture = TextureManager::GetInstance()->Load("Resources/UI/JumpUItt.png");
	data.tag = "UI" + std::to_string(uiNumber_);
	AddUI(data);
	data.num = uiNumber_;
	data.position = { 139.0f,240.0f };
	data.texture = TextureManager::GetInstance()->Load("Resources/UI/LockonUIt.png");
	data.tag = "UI" + std::to_string(uiNumber_);
	AddUI(data);
	data.num = uiNumber_;
	data.position = { 139.0f,120.0f };
	data.texture = TextureManager::GetInstance()->Load("Resources/UI/ShotUIt.png");
	data.tag = "UI" + std::to_string(uiNumber_);
	AddUI(data);

	//SpriteManager::LoadSprite
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

void GameScene::CameraUpdate()
{

	if (isDebugCamera_) {
		debugCamera_->Update();
		camera_.viewMatrix_ = debugCamera_->viewMatrix_;
		camera_.projectionMatrix_ = debugCamera_->projectionMatrix_;
		camera_.TransferMatrix();
	}
	else {
		// カメラの更新
		gameObjectManager_->GetFollowCamera()->Update();

		camera_.viewMatrix_ = gameObjectManager_->GetFollowCamera()->viewMatrix_;
		camera_.projectionMatrix_ = gameObjectManager_->GetFollowCamera()->projectionMatrix_;
		camera_.transform_ = gameObjectManager_->GetFollowCamera()->transform_;
		camera_.TransferMatrix();
	}
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
	lightData_.intensity = 1.5f;

	// 点光源データ
	ptLightData_.intensity = 5.0f;
	ptLightData_.position = { 0,50.0f,0 };
	ptLightData_.color = { 1,1,1,1 };
	ptLightData_.decay = 10.0f;
	ptLightData_.radius = 300.0f;

	// 照光源データ
	spLightData_.color = { 1,1,1,1 };
	spLightData_.position = { 2.0f,200.0f,0.0f };
	spLightData_.distance = 300.0f;
	spLightData_.direction = Vector3(-0.707f, -0.707f, 0.0f);
	spLightData_.intensity = 10.0f;
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

void GameScene::AddUI(UIData data)
{

	std::unique_ptr<Sprite> instance;
	instance.reset(Sprite::Create(data.texture, data.position, { 0.5f,0.5f }));
	std::pair<std::unique_ptr<Sprite>, UIData> par;
	par.first = std::move(instance);
	par.second = data;

	controlUIs_.push_back(std::move(par));

	uiNumber_++;
}
