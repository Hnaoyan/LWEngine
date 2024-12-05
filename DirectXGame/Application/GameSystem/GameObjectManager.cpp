#include "GameObjectManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Input/Input.h"
#include "Application/GameSystem/GameSystem.h"
#include <imgui.h>
#include <cassert>

GameObjectManager::GameObjectManager(GameSystem* system)
{
	// ゲームオブジェクト
	player_ = std::make_unique<Player>();
	boss_ = std::make_unique<Boss>();
	bulletManager_ = std::make_unique<BulletManager>();
	// 地形
	skyDome_ = std::make_unique<SkyDomeObject>();
	terrainManager_ = std::make_unique<TerrainManager>();

	gameSystem_ = system;
}

void GameObjectManager::Initialize(GPUParticleManager* gpuManager, ICamera* camera)
{
	// チェック
	assert(gpuManager);
	gpuManager_ = gpuManager;

	skyDome_->Initialize(ModelManager::GetModel("SkyDome"));
	// 弾
	bulletManager_->SetPlayer(player_.get());
	bulletManager_->SetBoss(boss_.get());
	bulletManager_->SetGPUParticle(gpuManager_);
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	// プレイヤー
	player_->PreInitialize(camera, gpuManager_);
	player_->Initialize(ModelManager::GetModel("Player"));
	player_->PointerInitialize(bulletManager_.get(), boss_.get(), nullptr);

	// ボス
	boss_->SetGPUParticle(gpuManager_);
	boss_->Initialize(ModelManager::GetModel("BossEnemy"));
	boss_->SetPlayer(player_.get());
	boss_->SetBulletManager(bulletManager_.get());
	boss_->SetCamera(camera);

	// 地形
	terrainManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	isSceneChange_ = false;
	isChangeInput_ = false;
	isInGame_ = true;
}

void GameObjectManager::Update()
{
	// ゲームの判断
#ifdef RELEASE
	if (gameSystem_->IsReplayMode()) {
		if ((player_->IsDead() || boss_->IsDead()) && isInGame_) {
			isInGame_ = false;
			waitingTimer_.Start(120.0f);
		}
	}
	else {
		// クリアしたタイミングの処理
		if (player_->IsDead() && isInGame_) {
			isInGame_ = false;
			waitingTimer_.Start(120.0f);
			gameOverTimer_.Start(120.0f);
		}
		if (boss_->IsDead() && isInGame_) {
			isInGame_ = false;
			waitingTimer_.Start(120.0f);
			gameClearTimer_.Start(120.0f);
		}
	}

	// UIが出ている時間
	gameClearTimer_.Update();
	gameOverTimer_.Update();
	waitingTimer_.Update();

	// ゲーム終了のタイミング
	if (waitingTimer_.IsEnd()) {
		if (gameSystem_->IsReplayMode()) {
			isSceneChange_ = true;
		}
		else {
			isChangeInput_ = true;
		}
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
	//if (!gameSystem_->IsReplayMode()) {
	//	if (PostEffectManager::sGameVigenette.timer.IsEnd()) {
	//		isGame = true;
	//		gameSystem_->GetReplayManager()->RecordSetUp();
	//	}
	//	if (!isGame) {
	//		return;
	//	}
	//}

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
	// ボス
	if (boss_) {
		boss_->Draw(drawDesc);
	}
	// プレイヤー
	player_->Draw(drawDesc);
}

void GameObjectManager::UIDraw()
{
	// それぞれのUI
	if (!gameSystem_->GetReplayManager()->IsReplayNow()) {
		player_->UISpriteDraw();
	}

	if (boss_) {
		boss_->UIDraw();
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

	// 地形
	// ゲームオブジェクト
	player_->ImGuiDraw();
	if (boss_) {
		boss_->ImGuiDraw();
	}
}

void GameObjectManager::RegisterCollider(CollisionManager* collisionManager)
{
	// 必須なのでチェック
	assert(collisionManager);

	// 全ての衝突設定
	if (player_) {
		player_->SetCollier(collisionManager);
	}
	if (boss_) {
		boss_->SetCollier(collisionManager);
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
	boss_->SetIsAction(true);
	boss_->GetSystem()->barrierManager_.Create(GlobalVariables::GetInstance()->GetValue<float>("Boss", "BarrierHP"));
}

void GameObjectManager::UpdateObject()
{
	// 地形関係
	skyDome_->Update();
	terrainManager_->Update();
	if (isInGame_) {
		// オブジェクト
		player_->Update();
		if (boss_) {
			boss_->Update();
		}
	}
	// アニメーションの処理
	if (boss_) {
		boss_->AnimationUpdate();
	}
	// 弾
	bulletManager_->Update();
}
