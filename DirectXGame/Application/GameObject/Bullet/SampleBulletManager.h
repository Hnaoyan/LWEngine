#pragma once
#include "SampleBullet.h"
#include <vector>

class CollisionManager;

/// <summary>
/// 弾のマネージャー
/// </summary>
class SampleBulletManager
{
public:
	/// <summary>
	/// 追加用のデータ
	/// </summary>
	struct GenerateData {
		Vector3 position;
		Vector3 velocity;
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
	void AddBullet(const GenerateData& data);

	void CollisionRegist();
public:	// アクセッサ
	void SetCollisionManager(CollisionManager* manager) {
		collisionManager_ = manager;
	}

private:
	// 全部のモデル
	Model* model_ = nullptr;
	// 弾のリスト
	std::vector<std::unique_ptr<SampleBullet>> bullets_;
	// 衝突マネ
	CollisionManager* collisionManager_ = nullptr;

};
