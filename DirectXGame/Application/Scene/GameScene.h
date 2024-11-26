#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Particle/GPUParticleSystem.h"

#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameSystem/UI/GameUIManager.h"
#include "Application/GameSystem/GameObjectManager.h"
#include "Application/GameSystem/Camera/CameraManager.h"

#include <optional>

class GameScene : public IScene
{
private:
	// ゲームシーンの状態
	enum class GameState
	{
		kWait,
		kGamePlay,
		kReplay,
	};

	// 変更リクエスト
	GameState nowState_ = GameState::kWait;
	std::optional<GameState> stateRequest_;

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
	/// ライト初期化
	/// </summary>
	void LightingInitialize();
	/// <summary>
	/// ライトの更新
	/// </summary>
	void LightingUpdate();
	/// <summary>
	/// 衝突処理
	/// </summary>
	void CollisionUpdate();

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
	std::unique_ptr<DirectionalLight> directionalLight_;
	CBufferDataDirectionalLight lightData_ = {};
	std::unique_ptr<SpotLight> spotLight_;
	CBufferDataSpotLight spLightData_ = {};
	std::unique_ptr<PointLight> pointLight_;
	CBufferDataPointLight ptLightData_ = {};
};
