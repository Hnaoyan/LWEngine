#pragma once
#include "Application/GameObject/GameObjectLists.h"

class CollisionManager;
class CameraManager;

/// <summary>
/// ゲームのオブジェクト管理クラス
/// </summary>
class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager() = default;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="gpuManager"></param>
	void Initialize(GPUParticleSystem* gpuManager, ICamera* camera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="lights"></param>
	void Draw(ICamera* camera, DrawDesc::LightDesc lights);
	/// <summary>
	/// UIの描画
	/// </summary>
	void UIDraw();
	/// <summary>
	/// ImGuiの処理
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// 当たり判定に登録する処理
	/// </summary>
	/// <param name="collisionManager"></param>
	void RegisterCollider(CollisionManager* collisionManager);
	/// <summary>
	/// ゲーム開始準備
	/// </summary>
	void GameSetUp();

public:
	// プレイヤー
	Player* GetPlayer() { return player_.get(); }

	// ゲームシーンベタ書き部分用のフラグ
	bool IsUIGameClear() { return gameClearTimer_.IsActive(); }
	bool IsUIGameOver() { return gameOverTimer_.IsActive(); }
	bool IsGameClear() { return isGameClear_; }
	bool IsGameOver() { return isGameOver_; }
private: // ゲームクリアなどの部分
	bool isGameOver_ = false;	// ゲームオーバー
	bool isGameClear_ = false;	// ゲームクリア
	bool isInGame_ = false;	// ゲーム内のプレイ中フラグ
	FrameTimer gameOverTimer_;
	FrameTimer gameClearTimer_;

private:
	// ゲームのユニット
	std::unique_ptr<Player> player_;
	std::unique_ptr<Boss> boss_;
	std::unique_ptr<BulletManager> bulletManager_;

	// 地形や背景
	std::unique_ptr<TerrainManager> terrainManager_;
	std::unique_ptr<SkyDomeObject> skyDome_;

	// マネージャーポインタ
	GPUParticleSystem* gpuManager_ = nullptr;
	
};
