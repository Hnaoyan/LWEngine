#include "GameObjectManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Input/Input.h"
#include "Application/GameSystem/GameSystem.h"
#include <imgui.h>
#include <cassert>

GameObjectManager::GameObjectManager(GameSystem* system)
{
	// チェック
	assert(system);
	gameSystem_ = system;
	// ゲームオブジェクト
	bulletManager_ = std::make_unique<BulletManager>();
	// 地形
	skyDome_ = std::make_unique<SkyDomeObject>();
	terrainManager_ = std::make_unique<TerrainManager>();
}

void GameObjectManager::Initialize(GPUParticleManager* gpuManager, ICamera* camera)
{
	// チェック
	assert(gpuManager);
	gameObjects_.clear();	// オブジェクトリセット
	gpuManager_ = gpuManager;

	skyDome_->Initialize(ModelManager::GetModel("SkyDome"));

	std::unique_ptr<Player> player = std::make_unique<Player>();
	std::unique_ptr<Boss> boss = std::make_unique<Boss>();

	// 弾
	bulletManager_->SetPlayer(player.get());
	bulletManager_->SetBoss(boss.get());
	bulletManager_->SetGPUParticle(gpuManager_);
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	// プレイヤー
	player->PreInitialize(camera, gpuManager_);
	player->Initialize(ModelManager::GetModel("Player"));
	player->PointerInitialize(bulletManager_.get(), boss.get(), nullptr);


	// ボス
	boss->SetGPUParticle(gpuManager_);
	boss->Initialize(ModelManager::GetModel("BossEnemy"));
	boss->SetPlayer(player.get());
	boss->SetBulletManager(bulletManager_.get());
	boss->SetCamera(camera);

	gameObjects_.emplace("Boss", std::move(boss));
	gameObjects_.emplace("Player", std::move(player));

	// 地形
	terrainManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	isSceneChange_ = false;
	isChangeInput_ = false;
	isInGame_ = true;
	isGameEnd_ = false;
	isClear_ = false;
}

void GameObjectManager::Update()
{
	// ゲームの判断
#ifdef RELEASE
	float waitFrame = 120.0f;
	if (gameSystem_->IsReplayMode()) {
		if ((gameObjects_["Player"]->IsDead() || gameObjects_["Boss"]->IsDead()) && isInGame_) {
			isInGame_ = false;
			waitingTimer_.Start(waitFrame);
		}
	}
	else {
		// クリアしたタイミングの処理
		if (gameObjects_["Player"]->IsDead() && isInGame_) {
			//isInGame_ = false;
			if (!waitingTimer_.IsActive()) {
				waitingTimer_.Start(waitFrame);
				isGameEnd_ = true;
				isClear_ = true;
			}
		}
		if (gameObjects_["Boss"]->IsDead() && isInGame_) {
			//isInGame_ = false;
			if (!waitingTimer_.IsActive()) {
				waitingTimer_.Start(waitFrame);
				isGameEnd_ = true;
				isClear_ = false;
			}
		}
	}

	// UIが出ている時間
	waitingTimer_.Update();

	// ゲーム終了のタイミング
	if (waitingTimer_.IsEnd()) {
		if (gameSystem_->IsReplayMode()) {
			isSceneChange_ = true;
		}
		else {
			isChangeInput_ = true;
		}
		isInGame_ = false;
	}
	
	// タイトルかリプレイかを選択できるように
	if (isChangeInput_) {
		if (Input::GetInstance()->XTriggerJoystick(XINPUT_GAMEPAD_A)) {
			isSceneReplay_ = true;
		}
		else if (Input::GetInstance()->XTriggerJoystick(XINPUT_GAMEPAD_B)) {
			isSceneChange_ = true;
		}
	}

	// リプレイ中に戻れるように
	if (gameSystem_->IsReplayMode()) {
		if (Input::GetInstance()->XTriggerJoystick(XINPUT_GAMEPAD_B) || gameSystem_->GetReplayManager()->IsReplayEnd()) {
			isSceneChange_ = true;
		}
	}

#endif // RELEASE

	// オブジェクトの更新
	UpdateObject();
}

void GameObjectManager::Draw(ICamera* camera, DrawDesc::LightDesc lights)
{
	// 描画に使うもの
	ModelDrawDesc drawDesc{};
	drawDesc.camera = camera;
	drawDesc.directionalLight = lights.directionalLight;
	drawDesc.pointLight = lights.pointLight;
	drawDesc.spotLight = lights.spotLight;
	// 球体
	skyDome_->Draw(drawDesc);
	// 地形
	terrainManager_->Draw(drawDesc);
	// 弾
	bulletManager_->Draw(drawDesc);
	// 描画
	for (std::unordered_map<std::string, std::unique_ptr<IGameObject>>::iterator it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
		(*it).second->Draw(drawDesc);
	}
}

void GameObjectManager::UIDraw()
{
	// それぞれのUI
	if (!gameSystem_->GetReplayManager()->IsReplayNow()) {
		// UI描画
		for (std::unordered_map<std::string, std::unique_ptr<IGameObject>>::iterator it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
			(*it).second->UIDraw();
		}
	}
}

void GameObjectManager::ImGuiDraw()
{	
	ImGui::Begin("GameObjectManager");
	if (ImGui::BeginTabBar("Object"))
	{
		if (ImGui::BeginTabItem("BulletManager")) {
			bulletManager_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Terrain")) {
			terrainManager_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("SkyDome")) {
			skyDome_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	for (std::unordered_map<std::string, std::unique_ptr<IGameObject>>::iterator it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
		(*it).second->ImGuiDraw();
	}
}

void GameObjectManager::RegisterCollider(CollisionManager* collisionManager)
{
	// 必須なのでチェック
	assert(collisionManager);

	// 全ての衝突設定
	for (std::unordered_map<std::string, std::unique_ptr<IGameObject>>::iterator it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
		(*it).second->SetCollier(collisionManager);
	}

	bulletManager_->CollisionUpdate(collisionManager);
	terrainManager_->CollisionUpdate(collisionManager);
}

void GameObjectManager::GameSetUp()
{
	// ポストエフェクト解除
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
	// 速度の初期化
	gameSystem_->sSpeedFactor = 1.0f;
	// ボスのセットアップ
	GetBoss()->SetIsAction(true);
	GetBoss()->GetSystem()->barrierManager_.Create(GlobalVariables::GetInstance()->GetValue<float>("Boss", "BarrierHP"));
}

void GameObjectManager::UpdateObject()
{
	// 天球
	skyDome_->Update();
	// ゲームのオブジェクト
	if (isInGame_) {
		// 更新
		for (std::unordered_map<std::string, std::unique_ptr<IGameObject>>::iterator it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
			(*it).second->Update();
		}
	}

	// 地形
	terrainManager_->Update();
	// 弾
	bulletManager_->Update();
}
