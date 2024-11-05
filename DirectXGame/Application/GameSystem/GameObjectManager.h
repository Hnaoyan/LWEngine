#pragma once
#include "Application/GameObject/GameObjectLists.h"

class CollisionManager;

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
	void Initialize(GPUParticleSystem* gpuManager);
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
public:
	ICamera* GetFollowCamera() { return followCamera_.get(); }

	bool IsUIGameClear() { return gameClearTimer_.IsActive(); }
	bool IsUIGameOver() { return gameOverTimer_.IsActive(); }

	bool IsGameClear() { return isGameClear_; }
	bool IsGameOver() { return isGameOver_; }
private:
	bool isGameOver_ = false;
	bool isGameClear_ = false;
	
	bool isInGame_ = false;

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

	// カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// マネージャーポインタ
	GPUParticleSystem* gpuManager_ = nullptr;
	
};
