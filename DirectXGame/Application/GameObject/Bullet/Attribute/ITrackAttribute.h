#pragma once
#include <memory>
#include "../Calc/TrackingAccelerater.h"

class IBullet;

/// <summary>
/// 追従の属性
/// </summary>
class ITrackiAttribute
{
public:
	virtual void Initialize(IBullet* bullet);
	virtual void Update();
private:
	virtual Vector3 CalculateDirection() = 0;

private:
	// 弾
	IBullet* bullet_ = nullptr;
	// 加速度計算用クラス
	std::unique_ptr<TrackingAccelerater> accelerater_;

};
