#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/Light/LightingManager.h"

#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameSystem/UI/GameUIManager.h"
#include "Application/GameSystem/GameObjectManager.h"
#include "Application/GameSystem/Camera/CameraManager.h"

#include <optional>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene
{
private:

	// 変更リクエスト
	GameSceneState nowState_ = GameSceneState::kWait;
	std::optional<GameSceneState> stateRequest_ = std::nullopt;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// UI描画
	/// </summary>
	void UIDraw() override;

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// モデル読み込み
	/// </summary>
	void LoadModel() override;

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture() override;
private:
	/// <summary>
	/// カメラ更新
	/// </summary>
	void CameraUpdate();
	/// <summary>
	/// 衝突処理
	/// </summary>
	void CollisionUpdate();

	/// <summary>
	/// ゲームの開始
	/// </summary>
	void BeginGame();
	void BeginTutorial();

	void SceneStateRequest(GameSceneState state) { stateRequest_ = state; }
	void ChangeState();

private: // アプリ
	// ゲーム
	std::unique_ptr<GameObjectManager> gameObjectManager_;	// オブジェクト
	std::unique_ptr<GameSystem> gameSystem_;	// システム
	// コリジョンマネ
	std::unique_ptr<CollisionManager> collisionManager_;
	// カメラマネ
	std::unique_ptr<CameraManager> cameraManager_;
	// GPUParticle
	std::unique_ptr<GPUParticleManager> gpuParticleManager_;
	// UI
	std::unique_ptr<GameUIManager> uiManager_;
private: // システム関係
	std::unique_ptr<LightingManager> lightManager_;

	Vector2 size_{256.0f,258.0f};
	Vector2 position_{160.0f,150.0f};
	Vector4 color_{ 0.0f,0.0f,0.0f,0.65f };
};
