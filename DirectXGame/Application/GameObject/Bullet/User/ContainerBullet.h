#pragma once
#include "../IBullet.h"
#include "../IBulletCluster.h"

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

	void SetCluster(IBulletCluster* cluster) { cluster_ = cluster; }

private:
	// 発射間隔を管理するタイマー
	FrameTimer fireTimer_;
	Vector3 moveDirect_{};

	int32_t deleteCount_ = 0;

	IBulletCluster* cluster_ = nullptr;

	void GenerateBullet();

};
