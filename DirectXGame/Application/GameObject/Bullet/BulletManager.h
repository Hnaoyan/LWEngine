#pragma once
#include "Engine/3D/Drawer/Model.h"
#include "BulletCluster.h"
#include <vector>

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
	void AddCluster();
public: // アクセッサ
	//std::vector<std::unique_ptr<InstancedGroup>> GetClusters() { return bulletClusters_; }
	BulletCluster* GetBeginCluster() {
		return static_cast<BulletCluster*>((*bulletClusters_.begin()).get());
	}
private:
	// モデルのリスト
	std::vector<Model*> models_;
	Model* model_ = nullptr;
	// 弾のリスト
	std::vector<std::unique_ptr<InstancedGroup>> bulletClusters_;

};
