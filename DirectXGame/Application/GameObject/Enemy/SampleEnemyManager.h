#pragma once
#include "SampleEnemy.h"

class CollisionManager;

class SampleEnemyManager
{
public:
	/// <summary>
	/// 追加用のデータ
	/// </summary>
	struct GenerateData {
		Vector3 position;
		//Vector3 velocity;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
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
	/// 追加
	/// </summary>
	/// <param name="data"></param>
	void AddEnemy(const GenerateData& data);

	/// <summary>
	/// 衝突マネージャーに設定
	/// </summary>
	void CollisionRegist();

public:	// アクセッサ
	void SetCollisionManager(CollisionManager* manager) {
		collisionManager_ = manager;
	}

private:
	// 全部のモデル
	Model* model_ = nullptr;
	// 弾のリスト
	std::vector<std::unique_ptr<SampleEnemy>> enemys_;
	// 衝突マネ
	CollisionManager* collisionManager_ = nullptr;

};
