#pragma once
#include "../IBullet.h"
#include "../IBulletCluster.h"

/// <summary>
/// コンテナ弾
/// </summary>
class ContainerBullet : public IBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// コールバック関数
	/// </summary>
	/// <param name="object"></param>
	void OnCollision(ColliderObject object) override;

public:	// アクセッサ
	void SetCluster(IBulletCluster* cluster) { cluster_ = cluster; }

private:
	// 発射間隔を管理するタイマー
	FrameTimer fireTimer_;
	// 移動方向
	Vector3 moveDirect_{};
	// 消した数
	int32_t deleteCount_ = 0;
	// 弾のクラスター
	IBulletCluster* cluster_ = nullptr;
	// 生成
	void GenerateBullet();

};
