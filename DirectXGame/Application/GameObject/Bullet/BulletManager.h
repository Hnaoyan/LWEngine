#pragma once
#include "Engine/3D/Drawer/Model.h"
#include "BulletCluster.h"
#include <vector>
#include <unordered_map>

class Player;
class Boss;

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
public: // USER
	/// <summary>
	/// クラスター作成
	/// </summary>
	void AddCluster(std::string tag);
public: // アクセッサ
	BulletCluster* FindCluster(std::string tag);

	void SetPlayer(Player* player) { player_ = player; }
	void SetBoss(Boss* boss) { boss_ = boss; }

private:
	// モデルのリスト
	std::vector<Model*> models_;
	Model* model_ = nullptr;

	// オブジェクトのポインタ
	Player* player_ = nullptr;
	Boss* boss_ = nullptr;

	// 弾のリスト
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>> clusters_;
};
