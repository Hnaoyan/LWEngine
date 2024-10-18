#pragma once
#include "Application/GameObject/GameObjectLists.h"

class CollisionManager;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager() = default;
public:
	void Initialize(GPUParticleSystem* gpuManager);
	void Update();
	void Draw(ICamera* camera, DrawDesc::LightDesc lights);
	void UIDraw();
	void ImGuiDraw();
	void RegisterCollider(CollisionManager* collisionManager);
public:
	ICamera* GetFollowCamera() { return followCamera_.get(); }

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
