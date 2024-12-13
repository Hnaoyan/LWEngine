#pragma once
#include "Engine/3D/Drawer/Model.h"
#include "IBulletCluster.h"
#include "Application/GameObject/Particle/User/Trail/TrailManager.h"
#include <vector>
#include <unordered_map>
#include <bitset>

class Player;
class Boss;
class GPUParticleManager;

/// <summary>
/// 弾の管理クラス
/// </summary>
class BulletManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc);
	/// <summary>
	/// 衝突更新
	/// </summary>
	/// <param name="manager"></param>
	void CollisionUpdate(CollisionManager* manager);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
public: // USER
	/// <summary>
	/// クラスター作成
	/// </summary>
	void AddCluster(const std::string& tag);
	void AddCluster(const std::string& tag, Model* model);
	void AddCluster(const std::string& tag, Model* model, uint32_t texture);

	/// <summary>
	/// クラスターの検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	IBulletCluster* FindCluster(const std::string& tag);
	/// <summary>
	/// クラスターの削除
	/// </summary>
	/// <param name="tag"></param>
	void DeleteCluster(const std::string& tag);
public: // アクセッサ


	// セッター
	void SetPlayer(Player* player) { player_ = player; }
	void SetBoss(Boss* boss) { boss_ = boss; }
	void SetGPUParticle(GPUParticleManager* gpuParticle) { gpuParticle_ = gpuParticle; }

private:
	// モデルのリスト
	std::vector<Model*> models_;
	Model* model_ = nullptr;

	// オブジェクトのポインタ
	Player* player_ = nullptr;
	Boss* boss_ = nullptr;
	GPUParticleManager* gpuParticle_ = nullptr;

	// 描画フラグ
	std::bitset<size_t(TrackingAttribute::kMaxSize)> isDrawCheck_;

	// 軌跡管理クラス
	std::unique_ptr<TrailManager> trailManager_;

	// 弾のリスト
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>> clusters_;
	std::vector<std::unique_ptr<InstancedGroup>> bombModel_;
};
