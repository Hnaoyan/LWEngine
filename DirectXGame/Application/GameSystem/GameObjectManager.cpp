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

	particleUnit_ = std::make_unique<TestParticle>();
}

void GameObjectManager::Initialize(GPUParticleManager* gpuManager, ICamera* camera)
{
	// チェック
	assert(gpuManager);
	gameObjects_.clear();	// オブジェクトリセット
	gpuManager_ = gpuManager;
	camera_ = camera;

	skyDome_->Initialize(ModelManager::GetModel("SkyDome"));

	// 弾
	bulletManager_->SetGPUParticle(gpuManager_);
	bulletManager_->Initialize(ModelManager::GetModel("BulletCube"));

	// 地形
	terrainManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	// パーティクル
	particleUnit_->Initialie(ModelManager::GetModel("TestPlane"));
	// フラグの初期化
	FlagReset();
}

void GameObjectManager::Update(GameSceneState state)
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
		if (state != GameSceneState::kGameTutorial) {
			// クリアしたタイミングの処理
			if (gameObjects_["Player"]->IsDead() && isInGame_) {
				//isInGame_ = false;
				if (!waitingTimer_.IsActive()) {
					waitingTimer_.Start(waitFrame);
					isGameEnd_ = true;
					isClear_ = false;
				}
			}
			if (gameObjects_["Boss"]->IsDead() && isInGame_) {
				//isInGame_ = false;
				if (!waitingTimer_.IsActive()) {
					waitingTimer_.Start(waitFrame);
					isGameEnd_ = true;
					isClear_ = true;
				}
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

	if (gameSystem_->GetReplayManager()->IsReplayEnd()) {
		isSceneChange_ = true;
	}

#endif // RELEASE
	state;
	// オブジェクトの更新
	UpdateObject();

	if (this->GetPlayer()) {
		particleUnit_->SetParent(GetPlayer()->GetWorldTransform());
	}
	particleUnit_->Update();
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
	// パーティクル
	particleUnit_->Draw(drawDesc);
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
	if (ImGui::TreeNode("Flags")) {
		ImGui::Checkbox("SceneChange", &isSceneChange_);
		ImGui::Checkbox("SceneReplay", &isSceneReplay_);
		ImGui::Checkbox("ChangeInput", &isChangeInput_);
		ImGui::Checkbox("InGame", &isInGame_);
		ImGui::Checkbox("GameEnd", &isGameEnd_);
		ImGui::Checkbox("Clear", &isClear_);
		ImGui::TreePop();
	}
	if (ImGui::Button("Paticle")) {
		particleUnit_->ActiveAception(300.0f);
	}
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
	// リストクリア
	gameObjects_.clear();
	// ポストエフェクト解除
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
	// 速度の初期化
	gameSystem_->sSpeedFactor = 1.0f;
	// 
	std::unique_ptr<Player> player = std::make_unique<Player>();
	std::unique_ptr<Boss> boss = std::make_unique<Boss>();
	bulletManager_->SetPlayer(player.get());
	bulletManager_->SetBoss(boss.get());
	// プレイヤー
	player->PreInitialize(camera_, gpuManager_);
	player->Initialize(ModelManager::GetModel("Player"));
	player->PointerInitialize(bulletManager_.get(), boss.get(), nullptr);
	// ボス
	boss->SetGPUParticle(gpuManager_);
	boss->Initialize(ModelManager::GetModel("BossEnemy"));
	boss->SetPlayer(player.get());
	boss->SetBulletManager(bulletManager_.get());
	boss->SetCamera(camera_);
	// リストに追加
	gameObjects_.emplace("Boss", std::move(boss));
	gameObjects_.emplace("Player", std::move(player));

	// クラスターの処理
	bulletManager_->ClusterClear();
	bulletManager_->PlayerCluster();
	bulletManager_->BossCluster();

	// フラグの初期化
	FlagReset();

	// ボスのセットアップ
	GetBoss()->SetIsAction(true);
	GetBoss()->GetSystem()->barrierManager_.Create(GlobalVariables::GetInstance()->GetValue<float>("Boss", "BarrierHP"));
}

void GameObjectManager::TutorialSetUp()
{
	// リストクリア
	gameObjects_.clear();
	// ポストエフェクト解除
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
	// 速度の初期化
	gameSystem_->sSpeedFactor = 1.0f;
	// 
	std::unique_ptr<Player> player = std::make_unique<Player>();
	bulletManager_->SetPlayer(player.get());
	// プレイヤー
	player->PreInitialize(camera_, gpuManager_);
	player->Initialize(ModelManager::GetModel("Player"));
	player->PointerInitialize(bulletManager_.get(), nullptr, nullptr);
	gameObjects_.emplace("Player", std::move(player));

	bulletManager_->ClusterClear();
	bulletManager_->PlayerCluster();

}

void GameObjectManager::FlagReset()
{
	isSceneReplay_ = false;
	isSceneChange_ = false;
	isChangeInput_ = false;
	isInGame_ = true;
	isGameEnd_ = false;
	isClear_ = false;
}

void GameObjectManager::UpdateObject()
{
	// 天球
	skyDome_->Update();

	// ゲームのオブジェクト
	if (isInGame_) {
		// 更新
		for (std::unordered_map<std::string, std::unique_ptr<IGameObject>>::iterator it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
			if ((*it).second) {
				(*it).second->Update();
			}
		}
	}

	// 地形
	terrainManager_->Update();
	// 弾
	bulletManager_->Update();
}
