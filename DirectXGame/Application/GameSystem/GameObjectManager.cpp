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
	// 地形
	skyDome_ = std::make_unique<SkyDomeObject>();

	particleUnit_ = std::make_unique<TestParticle>();
}

void GameObjectManager::Initialize(ICamera* camera)
{
	// チェック
	assert(camera);
	gameObjects_.clear();	// オブジェクトリセット
	camera_ = camera;

	skyDome_->Initialize(ModelManager::GetModel("SkyDome"));

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
	// パーティクル
	particleUnit_->Draw(drawDesc);

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

}

void GameObjectManager::GameSetUp()
{
	// リストクリア
	gameObjects_.clear();
	// ポストエフェクト解除
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
	// 速度の初期化
	gameSystem_->sSpeedFactor = 1.0f;

	// フラグの初期化
	FlagReset();

}

void GameObjectManager::TutorialSetUp()
{
	// リストクリア
	gameObjects_.clear();
	// ポストエフェクト解除
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
	// 速度の初期化
	gameSystem_->sSpeedFactor = 1.0f;

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
}
