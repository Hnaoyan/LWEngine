#include "GameScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"

#include <imgui.h>

void GameScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// ライト初期化
	LightingInitialize();
	// モデル読み込み
	LoadModel();
	// テクスチャ関係読み込み
	LoadTexture();

	gpuParticle_ = std::make_unique<GPUParticleSystem>();
	gpuParticle_->Initialize(ModelManager::GetModel("Plane"));

	collisionManager_ = std::make_unique<CollisionManager>();
	terrainManager_ = std::make_unique<TerrainManager>();
	terrainManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	player_ = std::make_unique<Player>();
	player_->Initialize(ModelManager::GetModel("Player"));

	bulletManager_ = std::make_unique<BulletManager>();
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"));


	enemyManager_ = std::make_unique<SampleEnemyManager>();
	enemyManager_->Initialize(ModelManager::GetModel("Enemy"));
	enemyManager_->SetCollisionManager(collisionManager_.get());

	bossEnemy_ = std::make_unique<Boss>();
	bossEnemy_->Initialize(ModelManager::GetModel("BossEnemy"));
	bossEnemy_->SetPlayer(player_.get());

	enemyManager_->AddEnemy({ 10.0f,0.0f,10.0f });
	enemyManager_->AddEnemy({ -10.0f,0.0f,10.0f });

	player_->SetBulletManager(bulletManager_.get());
	player_->SetBoss(bossEnemy_.get());

	terrainWtf_.Initialize();
	terrainWtf_.transform_.scale = { 20.0f,1.0f,20.0f };
	terrainWtf_.transform_.translate.y -= 0.5f;
#pragma region カメラ関係
	// 初期カメラ
	camera_.transform_.translate.y = 5.0f;
	camera_.transform_.rotate.x = 0.4f;
	camera_.transform_.translate.z = -7.0f;
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetParent(player_->GetWorldTransform());
	followCamera_->SetLockOn(player_->GetOperation()->GetLockOn());

	player_->SetFollowCamera(followCamera_.get());
	player_->SetEnemyList(enemyManager_->GetEnemysList());
#pragma endregion

}

void GameScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneManager_->ChangeScene("SAMPLE");
	}
#endif // _DEBUG


	terrainWtf_.UpdateMatrix();
	player_->Update();
	bulletManager_->Update();
	enemyManager_->Update();
	if (bossEnemy_) {
		bossEnemy_->Update();
		if (bossEnemy_->IsDead()) {
			bossEnemy_.reset();
			clearText_.isClear = true;
			clearText_.transitionTimer.Start(300.0f);
		}
	}

	if (clearText_.isClear) {
		clearText_.transitionTimer.Update();

		if (clearText_.transitionTimer.IsEnd()) {
			sceneManager_->ChangeScene("TITLE");
		}

	}

	terrainManager_->Update();

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

	Model::PreDraw(commandList);

	ModelDrawDesc desc{};
	desc.camera = &camera_;
	desc.directionalLight = directionalLight_.get();
	desc.pointLight = pointLight_.get();
	desc.spotLight = spotLight_.get();

	player_->Draw(desc);
	if (bossEnemy_) {
		bossEnemy_->Draw(desc);
	}
	enemyManager_->Draw(desc);

	bulletManager_->Draw(desc);

	// 地形
	terrainManager_->Draw(desc);

	desc.worldTransform = &terrainWtf_;
	//terrain_->Draw(desc);
	Model::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);

	player_->UISpriteDraw();

	Sprite::PostDraw();

#pragma endregion
}

void GameScene::UIDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	if (clearText_.isClear) {
		clearText_.clearText->Draw();
	}

	for (int i = 0; i < controlUIs_.size(); ++i) {
		controlUIs_[i].first->SetPosition(controlUIs_[i].second.position);
		controlUIs_[i].first->Draw();
	}

	Sprite::PostDraw();

}

void GameScene::ImGuiDraw()
{
#ifdef _DEBUG
	// ゲームオブジェクト
	player_->ImGuiDraw();
	if (bossEnemy_) {
		bossEnemy_->ImGuiDraw();
	}
	enemyManager_->ImGuiDraw();
	terrainManager_->ImGuiDraw();
	// カメラ
	camera_.ImGuiDraw();
	debugCamera_->ImGuiDraw();
	followCamera_->ImGuiDraw();
	enemyManager_->ImGuiDraw();
	ImGui::ShowDemoWindow();

	ImGui::Begin("SampleScene");

	if (ImGui::Button("BossRes")) {
		if (!bossEnemy_) {
			bossEnemy_ = std::make_unique<Boss>();
			bossEnemy_->Initialize(ModelManager::GetModel("DefaultCube"));
			bossEnemy_->SetPlayer(player_.get());
			player_->SetBoss(bossEnemy_.get());
		}
	}
	if (ImGui::Button("BossKill")) {
		if (bossEnemy_) {
			bossEnemy_->Dead();
		}
	}
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

#endif // _DEBUG
}

void GameScene::LoadModel()
{
	// モデルのロード
	ModelManager::LoadNormalModel("Terrain", "terrain");
	ModelManager::LoadNormalModel("Jett", "jett");
	ModelManager::LoadNormalModel("Enemy", "EnemyBug");
	ModelManager::LoadNormalModel("Sphere", "sphere");
	ModelManager::LoadNormalModel("BossEnemy", "BossEnemy");
	ModelManager::LoadNormalModel("Player", "Robotto");
	ModelManager::LoadObjModel("Plane", "plane");

	terrain_ = ModelManager::GetModel("Terrain");


}

void GameScene::LoadTexture()
{
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
	data.position = { 138.0f,187.0f };
	data.texture = TextureManager::GetInstance()->Load("Resources/UI/JumpUI.png");
	data.tag = "UI" + std::to_string(uiNumber_);
	AddUI(data);
	data.num = uiNumber_;
	data.position = { 153.0f,263.0f };
	data.texture = TextureManager::GetInstance()->Load("Resources/UI/LockonUI.png");
	data.tag = "UI" + std::to_string(uiNumber_);
	AddUI(data);
	data.num = uiNumber_;
	data.position = { 144.0f,120.0f };
	data.texture = TextureManager::GetInstance()->Load("Resources/UI/ShotUI.png");
	data.tag = "UI" + std::to_string(uiNumber_);
	AddUI(data);

}

void GameScene::CameraUpdate()
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
		camera_.TransferMatrix();
		// 基底更新
		//IScene::CameraUpdate();
	}
}

void GameScene::LightingInitialize()
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

void GameScene::CollisionUpdate()
{
	// クリア
	collisionManager_->ListClear();
	// 登録
	collisionManager_->ListRegist(player_->GetCollider());
	collisionManager_->ListRegist(player_->GetFootCollider());
	if (bossEnemy_) {
		collisionManager_->ListRegist(bossEnemy_->GetCollider());
		bossEnemy_->GetBulletManager()->CollisionUpdate(collisionManager_.get());
	}
	enemyManager_->CollisionRegist();
	bulletManager_->CollisionUpdate(collisionManager_.get());
	terrainManager_->CollisionUpdate(collisionManager_.get());

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
