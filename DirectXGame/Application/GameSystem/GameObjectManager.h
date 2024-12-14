#pragma once
#include "Application/GameObject/GameObjectLists.h"
#include <unordered_map>

class CollisionManager;
class CameraManager;
class GameSystem;

/// <summary>
/// ゲームのオブジェクト管理クラス
/// </summary>
class GameObjectManager
{
public:
	GameObjectManager(GameSystem* system);
	~GameObjectManager() = default;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="gpuManager"></param>
	void Initialize(GPUParticleManager* gpuManager, ICamera* camera);
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
	
	void UpdateObject();
public:
	// プレイヤー
	Player* GetPlayer() { return dynamic_cast<Player*>(gameObjects_["Player"].get()); }
	Boss* GetBoss() { return dynamic_cast<Boss*>(gameObjects_["Boss"].get()); }

	// ゲームシーンベタ書き部分用のフラグ
	bool IsUIGameClear() { return waitingTimer_.IsActive() && isClear_; }
	bool IsUIGameOver() { return waitingTimer_.IsActive() && !isClear_; }
	bool IsSceneChange() const { return isSceneChange_; }
	bool IsSceneReplay() const { return isSceneReplay_; }
	bool IsChange() const { return isChangeInput_; }
	bool IsGameEnd()const { return isGameEnd_; }

	void SetGameSystem(GameSystem* system) { gameSystem_ = system; }
	void SetGameEnd(bool isGameEnd) { isGameEnd_ = isGameEnd; }
private: // ゲームクリアなどの部分
	bool isSceneChange_ = false;	// シーンの切り替わりフラグ
	bool isSceneReplay_ = false;	// リプレイに行くフラグ
	bool isInGame_ = false;	// ゲーム内のプレイ中フラグ
	bool isChangeInput_ = false;	// シーン変更の受付フラグ
	bool isGameEnd_ = false;	// ゲーム終了確認フラグ
	bool isClear_ = false;	// クリアならtrue・ゲームオーバーならfalse

	FrameTimer waitingTimer_;	// ゲーム終了後の待機時間

	GameSystem* gameSystem_ = nullptr;
private:
	//std::vector<IGameObject> gameObjects_;
	std::unordered_map<std::string, std::unique_ptr<IGameObject>> gameObjects_;
	// ゲームのユニット
	std::unique_ptr<BulletManager> bulletManager_;

	// 地形や背景
	std::unique_ptr<TerrainManager> terrainManager_;
	std::unique_ptr<SkyDomeObject> skyDome_;

	// マネージャーポインタ
	GPUParticleManager* gpuManager_ = nullptr;
	
};
