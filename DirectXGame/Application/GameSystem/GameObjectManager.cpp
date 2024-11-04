#include "GameObjectManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Collision/CollisionManager.h"
#include <cassert>

GameObjectManager::GameObjectManager()
{
	// ゲームオブジェクト
	player_ = std::make_unique<Player>();
	boss_ = std::make_unique<Boss>();
	bulletManager_ = std::make_unique<BulletManager>();
	// 地形
	skyDome_ = std::make_unique<SkyDomeObject>();
	terrainManager_ = std::make_unique<TerrainManager>();
	// カメラ
	followCamera_ = std::make_unique<FollowCamera>();
}

void GameObjectManager::Initialize(GPUParticleSystem* gpuManager)
{
	// チェック
	assert(gpuManager);
	gpuManager_ = gpuManager;

	skyDome_->Initialize(ModelManager::GetModel("SkyDome"));
	// 弾
	bulletManager_->SetPlayer(player_.get());
	bulletManager_->SetBoss(boss_.get());
	bulletManager_->SetGPUParticle(gpuManager);
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	// プレイヤー
	player_->PreInitialize(followCamera_.get(), gpuManager_);
	player_->Initialize(ModelManager::GetModel("Player"));
	player_->PointerInitialize(bulletManager_.get(), boss_.get(), nullptr);

	// ボス
	boss_->SetGPUParticle(gpuManager);
	boss_->Initialize(ModelManager::GetModel("BossEnemy"));
	boss_->SetPlayer(player_.get());
	boss_->SetBulletManager(bulletManager_.get());

	// カメラ
	followCamera_->Initialize();
	followCamera_->SetParent(player_->GetWorldTransform());
	followCamera_->SetLockOn(player_->GetOperation()->GetLockOn());

	// 地形
	terrainManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	isGameClear_ = false;
	isGameOver_ = false;
	isInGame_ = true;
}

void GameObjectManager::Update()
{
	// ゲームの判断
#ifdef RELEASE

	if (player_->IsDead() && isInGame_) {
		isInGame_ = false;
		gameOverTimer_.Start(120.0f);
	}
	if (boss_->IsDead() && isInGame_) {
		isInGame_ = false;
		gameClearTimer_.Start(120.0f);
	}

	gameClearTimer_.Update();
	gameOverTimer_.Update();

	if (gameClearTimer_.IsEnd()) {
		isGameClear_ = true;
	}
	if (gameOverTimer_.IsEnd()) {
		isGameOver_ = true;
	}

#endif // RELEASE


	// 地形関係
	skyDome_->Update();
	terrainManager_->Update();
	//if (isInGame_) {
		// オブジェクト
		player_->Update();
		if (boss_) {
			boss_->Update();
		}
		bulletManager_->Update();
	//}
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
	// プレイヤー
	player_->Draw(drawDesc);
	// ボス
	if (boss_) {
		boss_->Draw(drawDesc);
	}
	// 弾
	bulletManager_->Draw(drawDesc);
}

void GameObjectManager::UIDraw()
{
	// それぞれのUI
	player_->UISpriteDraw();
	if (boss_) {
		boss_->UIDraw();
	}
}

void GameObjectManager::ImGuiDraw()
{
	// カメラ
	followCamera_->ImGuiDraw();
	// 地形
	terrainManager_->ImGuiDraw();
	skyDome_->ImGuiDraw();
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
	collisionManager->ListRegist(player_->GetCollider());
	collisionManager->ListRegist(player_->GetFootCollider());
	if (boss_) {
		collisionManager->ListRegist(boss_->GetCollider());
	}
	bulletManager_->CollisionUpdate(collisionManager);
	terrainManager_->CollisionUpdate(collisionManager);
}
