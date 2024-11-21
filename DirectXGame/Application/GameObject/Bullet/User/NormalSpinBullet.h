#pragma once
#include "../IBullet.h"

/// <summary>
/// 通常の弾
/// </summary>
class NormalSpinBullet : public IBullet
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
private:
	// 移動方向
	Vector3 moveDirect_{};

public: // 共通
	static float sAcceleration;

};