#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/Light/LightingManager.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/LevelEditor/LevelLoader.h"

#include "Engine/NetWork/Login/NetLogin.h"

// アプリ
#include "Application/NetWorkGame/NetworkGameObjectLists.h"

#include <optional>

class NetworkScene : public IScene
{
private:
	// ゲームの状態
	enum GameState {
		WAIT_START,
		RUNNING,
		RESULT,
		RANKING,
	};

	// 現在の状態
	std::optional<GameState> request_ = std::nullopt;
	GameState currentState_ = WAIT_START;
	// 時間
	float runningTime_ = 0.0f;
	// スコア
	int runningScore_ = 0;

	int ClearScore(const float& time);

	std::string rankingText_ = "";

	NetLogin loginAPI_;

	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Obstacle> obstacle_;
private:
	std::unique_ptr<LightingManager> lightManager_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// GPUの更新
	/// </summary>
	void GPUUpdate() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// UI
	/// </summary>
	void UIDraw() override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

private:
	/// <summary>
	/// リクエストの処理
	/// </summary>
	void RequestProcess();
};
