#include "NetworkScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/NetWork/Http/HttpClient.h"
#include "Engine/NetWork/Http/HttpScoreClient.h"

#include <imgui.h>
#include <json.hpp>

void NetworkScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();

	loginAPI_ = NetLogin("Naoya05", "password");

	// ライト初期化
	lightManager_ = std::make_unique<LightingManager>();
	lightManager_->Initialize();
	collisionManager_ = std::make_unique<CollisionManager>();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	player_ = std::make_unique<Player>();
	player_->Initialize(ModelManager::GetModel("Player"));

	goal_ = std::make_unique<GoalObject>();
	goal_->Initialize(ModelManager::GetModel("Cube"));

	obstacleManager_ = std::make_unique<ObstacleManager>();
	obstacleManager_->Initialize();

	timerSystem_ = std::make_unique<GameTimerSystem>();
	timerSystem_->Initialize();
}

void NetworkScene::GPUUpdate()
{
}

void NetworkScene::Update()
{
	// タイマー
	timerSystem_->Update();

	lightManager_->Update();
	debugCamera_->Update();

	// オブジェクト
	player_->Update();
	goal_->Update();
	obstacleManager_->Update();

	//NetworkUpdate();

	CollisionUpdate();

}

void NetworkScene::Draw()
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
	//DrawDesc::LightDesc lightDesc{};
	//lightDesc.directionalLight = lightManager_->GetDirectional();
	//lightDesc.pointLight = lightManager_->GetPoint();
	//lightDesc.spotLight = lightManager_->GetSpot();
	// 描画に使うもの
	ModelDrawDesc drawDesc{};
	drawDesc.camera = debugCamera_.get();
	drawDesc.directionalLight = lightManager_->GetDirectional();
	drawDesc.pointLight = lightManager_->GetPoint();
	drawDesc.spotLight = lightManager_->GetSpot();

	player_->Draw(drawDesc);
	goal_->Draw(drawDesc);
	obstacleManager_->Draw(drawDesc);
	ModelRenderer::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion
}

void NetworkScene::UIDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();
}

void NetworkScene::ImGuiDraw()
{
	// ログインAPI
	loginAPI_.ImGuiDraw();

	debugCamera_->ImGuiDraw();
	timerSystem_->ImGuiDraw();
	obstacleManager_->ImGuiDraw();
	player_->ImGuiDraw();
	goal_->ImGuiDraw();
	ImGui::Begin("NetWork");
	ImGui::InputInt("Score", &runningScore_);

	switch (currentState_)
	{
	case NetworkScene::WAIT_START:
		ImGui::Text("Press SPACE to Start");
		break;
	case NetworkScene::RUNNING:
		ImGui::Text("RUNNING");
		if (runningTime_ <= 7.0f) {
			ImGui::Text("%.2f", runningTime_);
		}
		break;
	case NetworkScene::RESULT:
		ImGui::Text("RESULT");
		ImGui::Text("%.2f", runningTime_);
		break;
	case NetworkScene::RANKING:
		ImGui::Text("RANKING");
		break;
	default:
		break;
	}


	ImGui::End();

	if (currentState_ == RANKING) {

		ImGui::Begin("RANKING");
		std::istringstream iss(rankingText_);
		std::string line;
		while (std::getline(iss, line)) {
			ImGui::Text(line.c_str());
		}

		ImGui::Text("Press SPACE to play again");

		ImGui::End();

	}

}

void NetworkScene::RequestProcess()
{
	if (request_) {
		GameState newState = request_.value();
		HttpScoreClient http;
		std::string postSync = "";
		std::string allScores = "";
		switch (newState)
		{
		case NetworkScene::WAIT_START:
			break;
		case NetworkScene::RUNNING:
			runningTime_ = 0.0f;
			break;
		case NetworkScene::RESULT:
			runningScore_ = ClearScore(runningTime_);
			break;
		case NetworkScene::RANKING:
			postSync = http.PostScoreAsync(runningScore_).get();
			allScores = http.GetAllScoreAsync().get();
			try
			{
				nlohmann::json j = nlohmann::json::parse(allScores);
				rankingText_ = "Ranking Top 5:\n";
				int i = 0;
				for (const auto& entry : j) {
					int rankScore = entry["score"];
					rankingText_ += std::to_string(i + 1) + ". " + std::to_string(rankScore) + "\n";
					++i;
				}
			}
			catch (const std::exception&)
			{
				rankingText_ = "[Ranking Error]";
			}
			break;
		default:
			break;
		}

		currentState_ = newState;
		request_ = std::nullopt;
	}
}

void NetworkScene::CollisionUpdate()
{
	// クリア
	collisionManager_->ListClear();

	// ゲームの登録
	player_->SetCollision(collisionManager_.get());
	goal_->SetCollision(collisionManager_.get());
	obstacleManager_->CollisionUpdate(collisionManager_.get());
	// 衝突処理
	collisionManager_->CheckAllCollisions();
}

void NetworkScene::NetworkUpdate()
{
	// リクエスト処理
	RequestProcess();

	if (false) {
		return;
	}

	switch (currentState_)
	{
	case NetworkScene::WAIT_START:
		// 変更
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			request_ = RUNNING;
			break;
		}

		break;
	case NetworkScene::RUNNING:
		// 変更
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			request_ = RESULT;
			break;
		}
		runningTime_ += kDeltaTime;

		break;
	case NetworkScene::RESULT:
		// 変更
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			request_ = RANKING;

			break;
		}
		break;
	case NetworkScene::RANKING:
		// 変更
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			request_ = WAIT_START;
			break;
		}
		break;
	default:
		break;
	}
}

int NetworkScene::ClearScore(const float& time)
{
	// 返り値
	int result = 1000;
	// 差分
	float dis = 10.0f - time;
	// ドボン（失敗）
	if (dis < 0) {
		return 0;
	}
	// ジャスト（成功）
	if (dis == 0.0f) return result;

	// 減算処理
	while (true) {
		dis -= 0.1f;
		if (dis < 0.01f) {
			break;
		}
		else {
			result -= 10;
		}
	}

	return result;
}
