#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "../GameObject/GameObjectLists.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameSystem/UI/GameUIManager.h"

class GameScene : public IScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

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

	/// <summary>
	/// カメラ更新
	/// </summary>
	void CameraUpdate() override;

	/// <summary>
	/// ライト初期化
	/// </summary>
	void LightingInitialize() override;
	/// <summary>
	/// ライトの更新
	/// </summary>
	void LightingUpdate();
private:
	/// <summary>
	/// 衝突処理
	/// </summary>
	void CollisionUpdate();

private: // アプリ
	std::unique_ptr<Player> player_;
	std::unique_ptr<SampleEnemyManager> enemyManager_;
	std::unique_ptr<Boss> bossEnemy_;
	std::unique_ptr<SkyDomeObject> skydome_;
	// ゲームのシステム関係	
	std::unique_ptr<GameSystem> gameSystem_;

	// 地形マネ
	std::unique_ptr<TerrainManager> terrainManager_;
	// 弾のマネージャー
	std::unique_ptr<BulletManager> bulletManager_;
	// コリジョンマネ
	std::unique_ptr<CollisionManager> collisionManager_;
	// GPUParticle
	std::unique_ptr<GPUParticleSystem> gpuParticleManager_;
	// UI
	std::unique_ptr<GameUI::UIManager> uiManager_;
private: // リソース
	std::unique_ptr<Sprite> reticleSprite_;

	// クリア関係の変数
	struct GameClear {
		FrameTimer transitionTimer;
		std::unique_ptr<Sprite> clearText;
		bool isClear;
	};

	FrameTimer backTitleTimer_;
	bool isGameOver_ = false;

	// UI用のデータ
	struct UIData {
		std::string tag;
		uint32_t num = 0;
		Vector2 position{};
		Vector2 scale{};
		uint32_t texture = 0;
	};

	std::vector<std::pair<std::unique_ptr<Sprite>, UIData>> controlUIs_;

	void AddUI(UIData data);

	uint32_t uiNumber_ = 0;

	GameClear clearText_;

private: // システム関係
	// カメラ君
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	bool isDebugCamera_ = false;

	Model* terrain_ = nullptr;
	WorldTransform terrainWtf_ = {};

	// ライト君
	std::unique_ptr<DirectionalLight> directionalLight_;
	CBufferDataDirectionalLight lightData_;
	std::unique_ptr<SpotLight> spotLight_;
	CBufferDataSpotLight spLightData_;
	std::unique_ptr<PointLight> pointLight_;
	CBufferDataPointLight ptLightData_;
};
