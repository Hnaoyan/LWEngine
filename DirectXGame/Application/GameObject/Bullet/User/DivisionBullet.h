#pragma once
#include "../IBullet.h"
#include "../IBulletCluster.h"

/// <summary>
/// 分裂する弾（今は使ってない
/// </summary>
class DivisionBullet : public IBullet
{
public:
	~DivisionBullet() override {
		missileCluster_ = nullptr;
	}

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

private:
	IBulletCluster* missileCluster_ = nullptr;

	// 分裂するまでの時間
	FrameTimer divisionTimer_;

};

